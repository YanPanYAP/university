#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const OUTPUT_DIR = path.join(__dirname, 'output');
const MESSAGE = 'Pavlovich Yan Andreevich';
const HASH_ALGORITHM = 'sha256';
const RSA_BITS = 2048;
const GROUP_BITS = 2048;
const ITERATIONS = 5;

function ensureOutputDir() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
}

function toMs(ns) {
  return Number(ns) / 1e6;
}

function measure(fn) {
  const start = process.hrtime.bigint();
  const value = fn();
  return { value, ms: toMs(process.hrtime.bigint() - start) };
}

function aggregate(values) {
  const sum = values.reduce((acc, value) => acc + value, 0);
  return {
    meanMs: sum / values.length,
    minMs: Math.min(...values),
    maxMs: Math.max(...values)
  };
}

function mod(n, m) {
  const r = n % m;
  return r < 0n ? r + m : r;
}

function egcd(a, b) {
  let oldR = BigInt(a);
  let r = BigInt(b);
  let oldS = 1n;
  let s = 0n;

  while (r !== 0n) {
    const q = oldR / r;
    [oldR, r] = [r, oldR - q * r];
    [oldS, s] = [s, oldS - q * s];
  }

  return { g: oldR, x: oldS };
}

function gcd(a, b) {
  let x = BigInt(a);
  let y = BigInt(b);
  while (y !== 0n) {
    [x, y] = [y, x % y];
  }
  return x < 0n ? -x : x;
}

function modInv(a, m) {
  const { g, x } = egcd(mod(a, m), m);
  if (g !== 1n) throw new Error('Обратный элемент не существует.');
  return mod(x, m);
}

function modPow(base, exp, modulus) {
  let b = mod(base, modulus);
  let e = BigInt(exp);
  let result = 1n;

  while (e > 0n) {
    if ((e & 1n) === 1n) result = (result * b) % modulus;
    b = (b * b) % modulus;
    e >>= 1n;
  }

  return result;
}

function randBigIntBelow(limit) {
  const max = BigInt(limit);
  const bits = max.toString(2).length;
  const bytes = Math.ceil(bits / 8);

  while (true) {
    const buf = crypto.randomBytes(bytes);
    const extraBits = bytes * 8 - bits;
    if (extraBits > 0) buf[0] &= 0xff >>> extraBits;
    const value = BigInt(`0x${buf.toString('hex')}`);
    if (value < max) return value;
  }
}

function randBigIntBetween(min, max) {
  const a = BigInt(min);
  const b = BigInt(max);
  return a + randBigIntBelow(b - a + 1n);
}

function randomCoprime(modulus) {
  let k = randBigIntBetween(2n, modulus - 2n);
  while (gcd(k, modulus) !== 1n) {
    k = randBigIntBetween(2n, modulus - 2n);
  }
  return k;
}

function sha256BigInt(...parts) {
  const hash = crypto.createHash(HASH_ALGORITHM);
  for (const part of parts) hash.update(String(part));
  return BigInt(`0x${hash.digest('hex')}`);
}

function sha256Hex(message) {
  return crypto.createHash(HASH_ALGORITHM).update(message).digest('hex');
}

function base64UrlToBuffer(value) {
  const normalized = value.replace(/-/g, '+').replace(/_/g, '/');
  return Buffer.from(normalized.padEnd(Math.ceil(normalized.length / 4) * 4, '='), 'base64');
}

function bufferToBigInt(buffer) {
  return BigInt(`0x${buffer.toString('hex') || '0'}`);
}

function bigIntToBuffer(value, length) {
  const hex = value.toString(16).padStart(length * 2, '0');
  return Buffer.from(hex, 'hex');
}

function preview(value, chars = 42) {
  const text = String(value);
  return text.length > chars ? `${text.slice(0, chars)}...` : text;
}

function generateSafeGroup(bits) {
  const p = crypto.generatePrimeSync(bits, { safe: true, bigint: true });
  const q = (p - 1n) / 2n;
  let h = 2n;
  let g = modPow(h, 2n, p);

  while (g === 1n) {
    h += 1n;
    g = modPow(h, 2n, p);
  }

  return { p, q, g };
}

function generateRSA() {
  const { publicKey, privateKey } = crypto.generateKeyPairSync('rsa', {
    modulusLength: RSA_BITS,
    publicExponent: 0x10001
  });
  return { publicKey, privateKey };
}

function rsaSign(message, key) {
  return crypto.sign(HASH_ALGORITHM, Buffer.from(message, 'utf8'), key);
}

function rsaVerify(message, signature, key) {
  return crypto.verify(HASH_ALGORITHM, Buffer.from(message, 'utf8'), key, signature);
}

function rsaVerifyTrace(message, signature, publicKey) {
  const jwk = publicKey.export({ format: 'jwk' });
  const n = bufferToBigInt(base64UrlToBuffer(jwk.n));
  const e = bufferToBigInt(base64UrlToBuffer(jwk.e));
  const expectedHex = sha256Hex(message);
  const encoded = bigIntToBuffer(modPow(bufferToBigInt(signature), e, n), Math.ceil(n.toString(2).length / 8));
  const digestInfoPrefix = Buffer.from('3031300d060960864801650304020105000420', 'hex');
  const prefixIndex = encoded.indexOf(digestInfoPrefix);
  const recoveredHex = prefixIndex >= 0
    ? encoded.subarray(prefixIndex + digestInfoPrefix.length, prefixIndex + digestInfoPrefix.length + 32).toString('hex')
    : '';

  return {
    expectedHex,
    recoveredHex,
    expectedDec: BigInt(`0x${expectedHex}`),
    recoveredDec: recoveredHex ? BigInt(`0x${recoveredHex}`) : 0n
  };
}

function generateElGamal(group) {
  const x = randBigIntBetween(2n, group.q - 1n);
  const y = modPow(group.g, x, group.p);
  return { ...group, x, y };
}

function elGamalSign(message, key) {
  const pMinusOne = key.p - 1n;
  const m = sha256BigInt(message) % pMinusOne;
  const k = randomCoprime(pMinusOne);
  const r = modPow(key.g, k, key.p);
  const s = mod((m - key.x * r) * modInv(k, pMinusOne), pMinusOne);
  return { r, s };
}

function elGamalVerify(message, signature, key) {
  const { r, s } = signature;
  if (r <= 0n || r >= key.p) return false;
  const m = sha256BigInt(message) % (key.p - 1n);
  const left = modPow(key.g, m, key.p);
  const right = mod(modPow(key.y, r, key.p) * modPow(r, s, key.p), key.p);
  return left === right;
}

function elGamalVerifyTrace(message, signature, key) {
  const { r, s } = signature;
  const m = sha256BigInt(message) % (key.p - 1n);
  const left = modPow(key.g, m, key.p);
  const right = mod(modPow(key.y, r, key.p) * modPow(r, s, key.p), key.p);
  return { m, left, right };
}

function generateSchnorr(group) {
  const x = randBigIntBetween(1n, group.q - 1n);
  const y = modPow(group.g, x, group.p);
  return { ...group, x, y };
}

function schnorrChallenge(message, r, q) {
  return sha256BigInt(message, '|', r) % q;
}

function schnorrSign(message, key) {
  const k = randBigIntBetween(1n, key.q - 1n);
  const r = modPow(key.g, k, key.p);
  const e = schnorrChallenge(message, r, key.q);
  const s = mod(k + key.x * e, key.q);
  return { e, s };
}

function schnorrVerify(message, signature, key) {
  const { e, s } = signature;
  if (e < 0n || e >= key.q || s < 0n || s >= key.q) return false;
  const yPow = modPow(key.y, e, key.p);
  const r = mod(modPow(key.g, s, key.p) * modInv(yPow, key.p), key.p);
  return schnorrChallenge(message, r, key.q) === e;
}

function schnorrVerifyTrace(message, signature, key) {
  const yPow = modPow(key.y, signature.e, key.p);
  const r = mod(modPow(key.g, signature.s, key.p) * modInv(yPow, key.p), key.p);
  const ePrime = schnorrChallenge(message, r, key.q);
  return { r, ePrime, e: signature.e };
}

function runBenchmark(name, signFn, verifyFn) {
  const signTimes = [];
  const verifyTimes = [];
  let signature;
  let valid = false;

  for (let i = 0; i < ITERATIONS; i += 1) {
    const sign = measure(signFn);
    signature = sign.value;
    signTimes.push(sign.ms);

    const verify = measure(() => verifyFn(signature));
    valid = verify.value;
    verifyTimes.push(verify.ms);
  }

  return {
    algorithm: name,
    signature,
    valid,
    sign: aggregate(signTimes),
    verify: aggregate(verifyTimes)
  };
}

function formatTable(headers, rows, alignments) {
  const widths = headers.map((header, col) => {
    const values = rows.map((row) => String(row[col]));
    return Math.max(String(header).length, ...values.map((value) => value.length), 3);
  });
  const line = (cells) => `| ${cells.map((cell, col) => {
    const text = String(cell);
    return alignments[col] === 'right' ? text.padStart(widths[col]) : text.padEnd(widths[col]);
  }).join(' | ')} |`;
  const separator = widths.map((width, col) => alignments[col] === 'right'
    ? `${'-'.repeat(width - 1)}:`
    : '-'.repeat(width));

  return [line(headers), line(separator), ...rows.map(line)].join('\n');
}

function buildReport({ message, group, generation, results, verifyTrace }) {
  const rows = results.map((result) => [
    result.algorithm,
    result.sign.meanMs.toFixed(6),
    result.verify.meanMs.toFixed(6),
    result.sign.minMs.toFixed(6),
    result.verify.minMs.toFixed(6),
    result.valid ? 'OK' : 'FAIL'
  ]);

  const publicRows = [
    ['RSA', `${RSA_BITS} бит`, 'PKCS#1/SHA-256', `${generation.rsa.toFixed(3)} ms`],
    ['Эль-Гамаль', `${GROUP_BITS} бит`, `p, g; q = (p - 1) / 2`, `${generation.elgamal.toFixed(3)} ms`],
    ['Шнорр', `${GROUP_BITS} бит`, `p, q, g`, `${generation.schnorr.toFixed(3)} ms`]
  ];

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`Сообщение: ${message}`);
  lines.push(`Хеш-функция: SHA-256`);
  lines.push(`Итераций на замер: ${ITERATIONS}`);
  lines.push(`Размер ключа RSA: ${RSA_BITS} бит`);
  lines.push(`Размер простого p для Эль-Гамаля и Шнорра: ${GROUP_BITS} бит`);
  lines.push(`Порядок подгруппы q: ${group.q.toString(2).length} бит`);
  lines.push('');
  lines.push('2. Сгенерированные параметры');
  lines.push(formatTable(
    ['Алгоритм', 'Параметры', 'Схема', 'Генерация'],
    publicRows,
    ['left', 'left', 'left', 'right']
  ));
  lines.push('');
  lines.push(`p = ${group.p}`);
  lines.push(`q = ${group.q}`);
  lines.push(`g = ${group.g}`);
  lines.push('');
  lines.push('3. Генерация и верификация ЭЦП');
  lines.push(formatTable(
    ['Алгоритм', 'Подпись mean ms', 'Проверка mean ms', 'Подпись min ms', 'Проверка min ms', 'Valid'],
    rows,
    ['left', 'right', 'right', 'right', 'right', 'left']
  ));
  lines.push('');
  lines.push('4. Формулы подсчета и численная проверка подписи');
  lines.push('t_mean = (t1 + t2 + t3 + t4 + t5) / 5');
  lines.push('t_min = min(t1, t2, t3, t4, t5)');
  lines.push('');
  lines.push(`H(M) = SHA-256("${message}")`);
  lines.push(`H(M) hex = ${verifyTrace.rsa.expectedHex}`);
  lines.push(`H(M) dec = ${verifyTrace.rsa.expectedDec}`);
  lines.push('');
  lines.push('RSA:');
  lines.push('H1 = SHA-256(M)');
  lines.push('H2 = RSA_public_verify(signature)');
  lines.push(`H1 dec = ${verifyTrace.rsa.expectedDec}`);
  lines.push(`H2 dec = ${verifyTrace.rsa.recoveredDec}`);
  lines.push(`H1 hex = ${verifyTrace.rsa.expectedHex}`);
  lines.push(`H2 hex = ${verifyTrace.rsa.recoveredHex}`);
  lines.push('');
  lines.push('Эль-Гамаль:');
  lines.push('m = H(M) mod (p - 1)');
  lines.push(`m = ${verifyTrace.elgamal.m}`);
  lines.push('left = g^m mod p');
  lines.push('right = (y^r * r^s) mod p');
  lines.push(`left = ${verifyTrace.elgamal.left}`);
  lines.push(`right = ${verifyTrace.elgamal.right}`);
  lines.push('');
  lines.push('Шнорр:');
  lines.push("r' = (g^s * (y^e)^(-1)) mod p");
  lines.push("e' = H(M | r') mod q");
  lines.push(`r' = ${verifyTrace.schnorr.r}`);
  lines.push(`e' = ${verifyTrace.schnorr.ePrime}`);
  lines.push(`e = ${verifyTrace.schnorr.e}`);
  lines.push('');
  lines.push('5. Контрольные значения подписи');
  for (const result of results) {
    lines.push(`${result.algorithm}:`);
    if (Buffer.isBuffer(result.signature)) {
      lines.push(`signature(base64) = ${result.signature.toString('base64')}`);
    } else {
      for (const [key, value] of Object.entries(result.signature)) {
        lines.push(`${key} = ${value}`);
      }
    }
    lines.push('');
  }

  return lines.join('\n');
}

function saveResults({ group, generation, results, report }) {
  ensureOutputDir();
  const json = {
    message: MESSAGE,
    hash: HASH_ALGORITHM,
    iterations: ITERATIONS,
    parameters: {
      rsaBits: RSA_BITS,
      groupBits: GROUP_BITS,
      p: group.p.toString(),
      q: group.q.toString(),
      g: group.g.toString()
    },
    generation,
    results: results.map((result) => ({
      algorithm: result.algorithm,
      valid: result.valid,
      sign: result.sign,
      verify: result.verify,
      signature: Buffer.isBuffer(result.signature)
        ? result.signature.toString('base64')
        : Object.fromEntries(Object.entries(result.signature).map(([key, value]) => [key, value.toString()]))
    }))
  };

  fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab10.txt'), report, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'results.json'), JSON.stringify(json, null, 2), 'utf8');
}

function main() {
  console.log('ЛР №10: электронная цифровая подпись');
  console.log(`Сообщение: ${MESSAGE}`);
  console.log('Генерация ключевых параметров...');

  const groupGeneration = measure(() => generateSafeGroup(GROUP_BITS));
  const group = groupGeneration.value;
  const rsaGeneration = measure(generateRSA);
  const elGamalGeneration = measure(() => generateElGamal(group));
  const schnorrGeneration = measure(() => generateSchnorr(group));

  const rsa = rsaGeneration.value;
  const elgamal = elGamalGeneration.value;
  const schnorr = schnorrGeneration.value;

  const results = [
    runBenchmark(
      'RSA',
      () => rsaSign(MESSAGE, rsa.privateKey),
      (signature) => rsaVerify(`${MESSAGE}!`, signature, rsa.publicKey)
    ),
    runBenchmark(
      'Эль-Гамаль',
      () => elGamalSign(MESSAGE, elgamal),
      (signature) => elGamalVerify(MESSAGE, signature, elgamal)
    ),
    runBenchmark(
      'Шнорр',
      () => schnorrSign(MESSAGE, schnorr),
      (signature) => schnorrVerify(MESSAGE, signature, schnorr)
    )
  ];

  const generation = {
    group: groupGeneration.ms,
    rsa: rsaGeneration.ms,
    elgamal: groupGeneration.ms + elGamalGeneration.ms,
    schnorr: groupGeneration.ms + schnorrGeneration.ms
  };
  const verifyTrace = {
    rsa: rsaVerifyTrace(MESSAGE, results[0].signature, rsa.publicKey),
    elgamal: elGamalVerifyTrace(MESSAGE, results[1].signature, elgamal),
    schnorr: schnorrVerifyTrace(MESSAGE, results[2].signature, schnorr)
  };
  const report = buildReport({ message: MESSAGE, group, generation, results, verifyTrace });

  saveResults({ group, generation, results, report });

  for (const result of results) {
    console.log(`${result.algorithm}: подпись ${result.sign.meanMs.toFixed(3)} ms, проверка ${result.verify.meanMs.toFixed(3)} ms, ${result.valid ? 'OK' : 'FAIL'}`);
  }
  console.log(`p: ${group.p.toString(2).length} бит, q: ${group.q.toString(2).length} бит`);
  console.log(`Отчет: ${path.join(OUTPUT_DIR, 'report_lab10.txt')}`);
  console.log(`JSON: ${path.join(OUTPUT_DIR, 'results.json')}`);
  console.log(`Пример подписи Шнорра: e=${preview(results[2].signature.e)}, s=${preview(results[2].signature.s)}`);
}

main();
