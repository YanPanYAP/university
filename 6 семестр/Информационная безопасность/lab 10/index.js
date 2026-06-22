#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const crypto = require('crypto');
const readline = require('readline/promises');
const { stdin: input, stdout: output } = require('process');

const OUTPUT_DIR = path.join(process.cwd(), 'output');
const ARTIFACTS_DIR = path.join(OUTPUT_DIR, 'artifacts');
const DEFAULT_FIO = 'Pavlovich Yan Andreevich';
const DEFAULT_ITERS = 8;
const A_VALUES = [7n, 31n];
const X_POWER_POINTS = [3, 17, 31, 45, 59, 73, 87, 100];
const KEY_SIZES = [1024, 2048];
const BASE64_ALPHABET = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';

function ensureDirs() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
  fs.mkdirSync(ARTIFACTS_DIR, { recursive: true });
}

function parseArgs(argv) {
  const args = { fio: null, iterations: null };
  for (let i = 0; i < argv.length; i += 1) {
    if (argv[i] === '--fio' && i + 1 < argv.length) {
      args.fio = argv[++i];
    } else if (argv[i] === '--iterations' && i + 1 < argv.length) {
      args.iterations = argv[++i];
    }
  }
  return args;
}

function ask(rl, question, fallback = '') {
  return rl.question(`${question}${fallback ? ` [${fallback}]` : ''}: `).then((answer) => {
    const trimmed = answer.trim();
    return trimmed || fallback;
  });
}

function gcd(a, b) {
  let x = BigInt(a);
  let y = BigInt(b);
  while (y !== 0n) {
    const t = x % y;
    x = y;
    y = t;
  }
  return x < 0n ? -x : x;
}

function egcd(a, b) {
  let oldR = BigInt(a);
  let r = BigInt(b);
  let oldS = 1n;
  let s = 0n;
  let oldT = 0n;
  let t = 1n;
  while (r !== 0n) {
    const q = oldR / r;
    [oldR, r] = [r, oldR - q * r];
    [oldS, s] = [s, oldS - q * s];
    [oldT, t] = [t, oldT - q * t];
  }
  return { g: oldR, x: oldS, y: oldT };
}

function mod(n, m) {
  const r = n % m;
  return r < 0n ? r + m : r;
}

function modInv(a, m) {
  const { g, x } = egcd(a, m);
  if (g !== 1n) throw new Error('Обратный элемент не существует.');
  return mod(x, m);
}

function modPow(base, exp, modN) {
  let b = mod(base, modN);
  let e = BigInt(exp);
  let r = 1n;
  while (e > 0n) {
    if (e & 1n) r = (r * b) % modN;
    b = (b * b) % modN;
    e >>= 1n;
  }
  return r;
}

function randBigIntBits(bits) {
  const bytes = Math.ceil(bits / 8);
  const buf = crypto.randomBytes(bytes);
  const extra = bytes * 8 - bits;
  if (extra > 0) {
    buf[0] &= (0xff >>> extra);
  }
  buf[0] |= (1 << (7 - extra));
  return BigInt(`0x${buf.toString('hex')}`);
}

function randBigIntBelow(limitExclusive) {
  const limit = BigInt(limitExclusive);
  if (limit <= 1n) return 0n;
  const bits = limit.toString(2).length;
  while (true) {
    const bytes = Math.ceil(bits / 8);
    const buf = crypto.randomBytes(bytes);
    const extra = bytes * 8 - bits;
    if (extra > 0) buf[0] &= (0xff >>> extra);
    const n = BigInt(`0x${buf.toString('hex')}`);
    if (n < limit) return n;
  }
}

function randBigIntBetween(min, max) {
  const a = BigInt(min);
  const b = BigInt(max);
  if (b < a) throw new Error('Неверный диапазон генерации.');
  return a + randBigIntBelow(b - a + 1n);
}

function randomOddBigInt(bits) {
  let n = randBigIntBits(bits);
  n |= 1n;
  return n;
}

function randomBigIntInRange(min, max) {
  return randBigIntBetween(min, max);
}

function bitLength(n) {
  return BigInt(n).toString(2).length;
}

function millerRabin(n, rounds = 16) {
  const x = BigInt(n);
  if (x < 2n) return false;
  if (x === 2n || x === 3n) return true;
  if (x % 2n === 0n) return false;

  let d = x - 1n;
  let s = 0n;
  while ((d & 1n) === 0n) {
    d >>= 1n;
    s += 1n;
  }

  for (let i = 0; i < rounds; i += 1) {
    const a = randomBigIntInRange(2n, x - 2n);
    let y = modPow(a, d, x);
    if (y === 1n || y === x - 1n) continue;
    let witness = true;
    for (let r = 1n; r < s; r += 1n) {
      y = modPow(y, 2n, x);
      if (y === x - 1n) {
        witness = false;
        break;
      }
    }
    if (witness) return false;
  }
  return true;
}

function generatePrime(bits) {
  let candidate = randomOddBigInt(bits);
  while (!millerRabin(candidate)) {
    candidate += 2n;
    if (bitLength(candidate) > bits) candidate = randomOddBigInt(bits);
  }
  return candidate;
}

function nextProbablePrime(start) {
  let n = BigInt(start);
  if (n <= 2n) return 2n;
  if (n % 2n === 0n) n += 1n;
  while (!millerRabin(n, 12)) n += 2n;
  return n;
}

function chooseE(phi) {
  const opts = [65537n, 17n, 3n];
  for (const e of opts) {
    if (gcd(e, phi) === 1n) return e;
  }
  let e = 5n;
  while (gcd(e, phi) !== 1n) e += 2n;
  return e;
}

function generateRSAKeys(bits) {
  const half = Math.floor(bits / 2);
  let p = generatePrime(half);
  let q = generatePrime(half);
  while (q === p) q = generatePrime(half);
  const n = p * q;
  const phi = (p - 1n) * (q - 1n);
  const e = chooseE(phi);
  const d = modInv(e, phi);
  return { bits, p, q, n, phi, e, d };
}

function rsaEncryptUnits(units, pub) {
  return units.map((m) => modPow(BigInt(m), pub.e, pub.n));
}

function rsaDecryptUnits(cipher, priv) {
  return cipher.map((c) => Number(modPow(c, priv.d, priv.n)));
}

function generateElGamalKeys(bits) {
  const p = generatePrime(bits);
  const g = randomBigIntInRange(2n, p - 2n);
  const x = randomBigIntInRange(2n, p - 2n);
  const y = modPow(g, x, p);
  return { bits, p, g, x, y };
}

function randomCoprime(limit) {
  const m = BigInt(limit);
  let k = randomBigIntInRange(2n, m - 1n);
  while (gcd(k, m) !== 1n) {
    k = randomBigIntInRange(2n, m - 1n);
  }
  return k;
}

function elgamalEncryptUnits(units, key) {
  const out = [];
  for (const unit of units) {
    const m = BigInt(unit);
    const k = randomCoprime(key.p - 1n);
    const a = modPow(key.g, k, key.p);
    const b = mod(modPow(key.y, k, key.p) * m, key.p);
    out.push([a, b]);
  }
  return out;
}

function elgamalDecryptUnits(cipherPairs, key) {
  const out = [];
  for (const [a, b] of cipherPairs) {
    const s = modPow(a, key.x, key.p);
    const sInv = modInv(s, key.p);
    const m = mod(b * sInv, key.p);
    out.push(Number(m));
  }
  return out;
}

function encodeASCII(text) {
  return {
    encoding: 'ASCII',
    units: Array.from(Buffer.from(text, 'utf8').values()),
    encodedText: text
  };
}

function decodeASCII(units) {
  return Buffer.from(units).toString('utf8');
}

function encodeBase64(text) {
  const b64 = Buffer.from(text, 'utf8').toString('base64').replace(/=+$/g, '');
  const units = [];
  for (const ch of b64) units.push(BASE64_ALPHABET.indexOf(ch));
  return { encoding: 'Base64', units, encodedText: b64 };
}

function decodeBase64(units) {
  const chars = units.map((u) => BASE64_ALPHABET[u]).join('');
  const pad = (4 - (chars.length % 4)) % 4;
  return Buffer.from(chars + '='.repeat(pad), 'base64').toString('utf8');
}

function toMs(ns) {
  return Number(ns) / 1e6;
}

function aggregate(msList) {
  const s = msList.reduce((acc, x) => acc + x, 0);
  return {
    meanMs: s / msList.length,
    minMs: Math.min(...msList),
    maxMs: Math.max(...msList)
  };
}

function buildXValues() {
  const out = [];
  for (const k of X_POWER_POINTS) {
    const base = 10n ** BigInt(k);
    out.push(nextProbablePrime(base + 1009n));
  }
  return out;
}

function runModexpBenchmark() {
  const nValues = [randBigIntBits(1024), randBigIntBits(2048)];
  const xValues = buildXValues();
  const rows = [];
  for (const a of A_VALUES) {
    for (const x of xValues) {
      for (let i = 0; i < nValues.length; i += 1) {
        const n = nValues[i];
        const bitN = i === 0 ? 1024 : 2048;
        const times = [];
        let y = 0n;
        for (let run = 0; run < 5; run += 1) {
          const t1 = process.hrtime.bigint();
          y = modPow(a, x, n);
          const t2 = process.hrtime.bigint();
          times.push(toMs(t2 - t1));
        }
        const agg = aggregate(times);
        rows.push({
          a: a.toString(),
          xDigits: x.toString().length,
          x: x.toString(),
          nBits: bitN,
          meanMs: agg.meanMs,
          minMs: agg.minMs,
          maxMs: agg.maxMs,
          yPreview: y.toString().slice(0, 32)
        });
      }
    }
  }
  return rows;
}

function rsaRoundtrip(units, key, iterations) {
  const encTimes = [];
  const decTimes = [];
  let cipher = [];
  let decrypted = [];
  for (let i = 0; i < iterations; i += 1) {
    const t1 = process.hrtime.bigint();
    cipher = rsaEncryptUnits(units, key);
    const t2 = process.hrtime.bigint();
    decrypted = rsaDecryptUnits(cipher, key);
    const t3 = process.hrtime.bigint();
    encTimes.push(toMs(t2 - t1));
    decTimes.push(toMs(t3 - t2));
  }
  return { cipher, decrypted, enc: aggregate(encTimes), dec: aggregate(decTimes) };
}

function elgamalRoundtrip(units, key, iterations) {
  const encTimes = [];
  const decTimes = [];
  let cipher = [];
  let decrypted = [];
  for (let i = 0; i < iterations; i += 1) {
    const t1 = process.hrtime.bigint();
    cipher = elgamalEncryptUnits(units, key);
    const t2 = process.hrtime.bigint();
    decrypted = elgamalDecryptUnits(cipher, key);
    const t3 = process.hrtime.bigint();
    encTimes.push(toMs(t2 - t1));
    decTimes.push(toMs(t3 - t2));
  }
  return { cipher, decrypted, enc: aggregate(encTimes), dec: aggregate(decTimes) };
}

function sizeMetrics(plainText, algorithm, keyBits, blocksCount, pairMode = false) {
  const plainBytes = Buffer.from(plainText, 'utf8').length;
  const modBytes = Math.ceil(keyBits / 8);
  const cipherBytes = pairMode ? blocksCount * 2 * modBytes : blocksCount * modBytes;
  const ratio = cipherBytes / plainBytes;
  const growthPct = (ratio - 1) * 100;
  return { algorithm, keyBits, plainBytes, cipherBytes, ratio, growthPct };
}

function formatTable(headers, rows, alignments) {
  const widths = headers.map((header, col) => {
    const values = rows.map((row) => String(row[col]));
    return Math.max(3, String(header).length, ...values.map((value) => value.length));
  });
  const padCell = (value, col) => {
    const text = String(value);
    return alignments[col] === 'right' ? text.padStart(widths[col]) : text.padEnd(widths[col]);
  };
  const separator = widths.map((width, col) => {
    return alignments[col] === 'right' ? `${'-'.repeat(width - 1)}:` : '-'.repeat(width);
  });
  const line = (cells) => `| ${cells.map(padCell).join(' | ')} |`;
  return [line(headers), line(separator), ...rows.map(line)];
}

function saveModexp(rows) {
  const csvHeader = 'a,x_digits,x,n_bits,mean_ms,min_ms,max_ms,y_preview';
  const csvRows = rows.map((r) =>
    [r.a, r.xDigits, r.x, r.nBits, r.meanMs.toFixed(6), r.minMs.toFixed(6), r.maxMs.toFixed(6), r.yPreview].join(',')
  );
  fs.writeFileSync(path.join(OUTPUT_DIR, 'modexp_table.csv'), [csvHeader, ...csvRows].join('\n'), 'utf8');

  const md = [];
  md.push('# Таблица времени вычисления y = a^x mod n');
  md.push('');
  md.push(...formatModexpTable(rows));
  fs.writeFileSync(path.join(OUTPUT_DIR, 'modexp_table.txt'), md.join('\n'), 'utf8');
}

function runAsymBench(fio, iterations) {
  const rsaResults = [];
  const elgResults = [];
  const comparison = [];

  for (const bits of KEY_SIZES) {
    const rsa = generateRSAKeys(bits);
    const elg = generateElGamalKeys(bits);

    for (const encMode of ['ASCII', 'Base64']) {
      const encoded = encMode === 'ASCII' ? encodeASCII(fio) : encodeBase64(fio);
      const decodeFn = encMode === 'ASCII' ? decodeASCII : decodeBase64;

      const rsaRt = rsaRoundtrip(encoded.units, rsa, iterations);
      const rsaText = decodeFn(rsaRt.decrypted);
      const rsaOk = rsaText === fio;
      const rsaSize = sizeMetrics(fio, 'RSA', bits, rsaRt.cipher.length, false);
      rsaResults.push({
        algorithm: 'RSA',
        bits,
        encoding: encMode,
        key: { n: rsa.n.toString(), e: rsa.e.toString(), d: rsa.d.toString() },
        sample: {
          plainUnit: encoded.units[0],
          cipherUnit: rsaRt.cipher[0].toString(),
          decryptedUnit: rsaRt.decrypted[0]
        },
        encrypt: rsaRt.enc,
        decrypt: rsaRt.dec,
        size: rsaSize,
        valid: rsaOk
      });

      const elgRt = elgamalRoundtrip(encoded.units, elg, iterations);
      const elgText = decodeFn(elgRt.decrypted);
      const elgOk = elgText === fio;
      const elgSize = sizeMetrics(fio, 'ElGamal', bits, elgRt.cipher.length, true);
      elgResults.push({
        algorithm: 'ElGamal',
        bits,
        encoding: encMode,
        key: { p: elg.p.toString(), g: elg.g.toString(), y: elg.y.toString(), x: elg.x.toString() },
        sample: {
          plainUnit: encoded.units[0],
          cipherA: elgRt.cipher[0][0].toString(),
          cipherB: elgRt.cipher[0][1].toString(),
          decryptedUnit: elgRt.decrypted[0]
        },
        encrypt: elgRt.enc,
        decrypt: elgRt.dec,
        size: elgSize,
        valid: elgOk
      });

      fs.writeFileSync(
        path.join(ARTIFACTS_DIR, `rsa_${bits}_${encMode.toLowerCase()}_cipher.txt`),
        rsaRt.cipher.map(String).join(' '),
        'utf8'
      );
      fs.writeFileSync(
        path.join(ARTIFACTS_DIR, `rsa_${bits}_${encMode.toLowerCase()}_decrypted.txt`),
        rsaText,
        'utf8'
      );
      fs.writeFileSync(
        path.join(ARTIFACTS_DIR, `elgamal_${bits}_${encMode.toLowerCase()}_cipher.txt`),
        elgRt.cipher.map(([a, b]) => `(${a},${b})`).join(' '),
        'utf8'
      );
      fs.writeFileSync(
        path.join(ARTIFACTS_DIR, `elgamal_${bits}_${encMode.toLowerCase()}_decrypted.txt`),
        elgText,
        'utf8'
      );

      comparison.push({
        algorithm: 'RSA',
        bits,
        encoding: encMode,
        encMeanMs: rsaRt.enc.meanMs,
        decMeanMs: rsaRt.dec.meanMs,
        plaintextBytes: rsaSize.plainBytes,
        ciphertextBytes: rsaSize.cipherBytes,
        ratio: rsaSize.ratio,
        growthPct: rsaSize.growthPct,
        valid: rsaOk
      });
      comparison.push({
        algorithm: 'ElGamal',
        bits,
        encoding: encMode,
        encMeanMs: elgRt.enc.meanMs,
        decMeanMs: elgRt.dec.meanMs,
        plaintextBytes: elgSize.plainBytes,
        ciphertextBytes: elgSize.cipherBytes,
        ratio: elgSize.ratio,
        growthPct: elgSize.growthPct,
        valid: elgOk
      });
    }
  }

  return { rsaResults, elgResults, comparison };
}

function saveComparison(comparison) {
  const md = [];
  md.push('# Сравнение RSA и Эль-Гамаля');
  md.push('');
  md.push(...formatComparisonTable(comparison, 'Alg'));
  fs.writeFileSync(path.join(OUTPUT_DIR, 'comparison_table.txt'), md.join('\n'), 'utf8');
}

function formatModexpTable(rows) {
  return formatTable(
    ['a', 'digits(x)', 'n bits', 'mean ms', 'min ms', 'max ms'],
    rows.map((r) => [r.a, r.xDigits, r.nBits, r.meanMs.toFixed(6), r.minMs.toFixed(6), r.maxMs.toFixed(6)]),
    ['right', 'right', 'right', 'right', 'right', 'right']
  );
}

function formatAlgorithmTable(rows) {
  return formatTable(
    ['Key bits', 'Encoding', 'Enc mean ms', 'Dec mean ms', 'Cipher bytes', 'Ratio', 'Valid'],
    rows.map((r) => [
      r.bits,
      r.encoding,
      r.encrypt.meanMs.toFixed(6),
      r.decrypt.meanMs.toFixed(6),
      r.size.cipherBytes,
      r.size.ratio.toFixed(2),
      r.valid ? 'OK' : 'FAIL'
    ]),
    ['right', 'left', 'right', 'right', 'right', 'right', 'left']
  );
}

function formatComparisonTable(comparison, firstHeader = 'Algorithm') {
  return formatTable(
    [firstHeader, 'Key bits', 'Encoding', 'Enc mean ms', 'Dec mean ms', 'Plain bytes', 'Cipher bytes', 'Ratio', 'Growth %', 'Valid'],
    comparison.map((r) => [
      r.algorithm,
      r.bits,
      r.encoding,
      r.encMeanMs.toFixed(6),
      r.decMeanMs.toFixed(6),
      r.plaintextBytes,
      r.ciphertextBytes,
      r.ratio.toFixed(2),
      r.growthPct.toFixed(2),
      r.valid ? 'OK' : 'FAIL'
    ]),
    ['left', 'right', 'left', 'right', 'right', 'right', 'right', 'right', 'right', 'left']
  );
}

function appendStepTrace(lines, fio, rsaResults, elgResults) {
  const firstChar = Array.from(fio)[0] || '';
  const asciiUnit = Buffer.from(firstChar, 'utf8')[0];
  const base64Text = Buffer.from(fio, 'utf8').toString('base64').replace(/=+$/g, '');
  const base64Char = base64Text[0] || '';
  const base64Unit = BASE64_ALPHABET.indexOf(base64Char);

  lines.push('## 2. Промежуточные вычисления');
  lines.push('');
  lines.push('Кодирование текста:');
  lines.push(`- ASCII: первый символ '${firstChar}' -> m = ${asciiUnit};`);
  lines.push(`- Base64: текст -> ${base64Text}, первый символ '${base64Char}' -> m = ${base64Unit}.`);
  lines.push('');

  for (const bits of KEY_SIZES) {
    const rsa = rsaResults.find((r) => r.bits === bits && r.encoding === 'ASCII');
    if (!rsa?.sample) continue;
    const n = BigInt(rsa.key.n);
    const e = BigInt(rsa.key.e);
    const d = BigInt(rsa.key.d);
    const c = BigInt(rsa.sample.cipherUnit);
    const decrypted = modPow(c, d, n);
    lines.push(`RSA, ключ ${bits} бит, ASCII, первый блок:`);
    lines.push(`n = ${n}`);
    lines.push(`e = ${e}`);
    lines.push(`d = ${d}`);
    lines.push(`m = ${rsa.sample.plainUnit}`);
    lines.push(`c = m^e mod n = ${c}`);
    lines.push(`m' = c^d mod n = ${decrypted}`);
    lines.push(`${decrypted} -> '${String.fromCharCode(Number(decrypted))}'`);
    lines.push('');
  }

  for (const bits of KEY_SIZES) {
    const elg = elgResults.find((r) => r.bits === bits && r.encoding === 'ASCII');
    if (!elg?.sample) continue;
    const p = BigInt(elg.key.p);
    const x = BigInt(elg.key.x);
    const a = BigInt(elg.sample.cipherA);
    const b = BigInt(elg.sample.cipherB);
    const s = modPow(a, x, p);
    const sInv = modInv(s, p);
    const decrypted = mod(b * sInv, p);
    lines.push(`Эль-Гамаль, ключ ${bits} бит, ASCII, первый блок:`);
    lines.push(`p = ${p}`);
    lines.push(`g = ${elg.key.g}`);
    lines.push(`x = ${x}`);
    lines.push(`y = g^x mod p = ${elg.key.y}`);
    lines.push(`m = ${elg.sample.plainUnit}`);
    lines.push(`a = g^k mod p = ${a}`);
    lines.push(`b = y^k * m mod p = ${b}`);
    lines.push(`s = a^x mod p = ${s}`);
    lines.push(`s^(-1) mod p = ${sInv}`);
    lines.push(`m' = b * s^(-1) mod p = ${decrypted}`);
    lines.push(`${decrypted} -> '${String.fromCharCode(Number(decrypted))}'`);
    lines.push('');
  }
}

function saveReport({ fio, iterations, modexpRows, rsaResults, elgResults, comparison }) {
  const allValid = [...rsaResults, ...elgResults].every((x) => x.valid);
  const avgRsaEnc = rsaResults.reduce((s, x) => s + x.encrypt.meanMs, 0) / rsaResults.length;
  const avgElgEnc = elgResults.reduce((s, x) => s + x.encrypt.meanMs, 0) / elgResults.length;
  const avgRsaDec = rsaResults.reduce((s, x) => s + x.decrypt.meanMs, 0) / rsaResults.length;
  const avgElgDec = elgResults.reduce((s, x) => s + x.decrypt.meanMs, 0) / elgResults.length;
  const avgRsaRatio = comparison.filter((x) => x.algorithm === 'RSA').reduce((s, x) => s + x.ratio, 0) / 4;
  const avgElgRatio = comparison.filter((x) => x.algorithm === 'ElGamal').reduce((s, x) => s + x.ratio, 0) / 4;

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`ФИО: ${fio}`);
  lines.push(`Итераций на замер: ${iterations}`);
  lines.push('Алгоритмы: RSA, Эль-Гамаль');
  lines.push('Кодировки: ASCII, Base64');
  lines.push('Размеры ключей: 1024 и 2048 бит');
  lines.push('Параметры modexp: y = a^x mod n,');
  lines.push('a = 7 и 31,');
  lines.push('x от 10^3 до 10^100,');
  lines.push('n = 1024 и 2048 бит');
  lines.push(`Корректность расшифрования: ${allValid ? 'OK' : 'есть ошибки'}`);
  lines.push('');
  appendStepTrace(lines, fio, rsaResults, elgResults);
  lines.push('## 3. Возведение в степень по модулю');
  lines.push('');
  lines.push(...formatModexpTable(modexpRows));
  lines.push('');
  lines.push('## 4. RSA');
  lines.push('');
  lines.push(...formatAlgorithmTable(rsaResults));
  lines.push('');
  lines.push(`Среднее время шифрования RSA: ${avgRsaEnc.toFixed(6)} ms`);
  lines.push(`Среднее время расшифрования RSA: ${avgRsaDec.toFixed(6)} ms`);
  lines.push(`Средний рост объема RSA: x${avgRsaRatio.toFixed(2)}`);
  lines.push('');
  lines.push('## 5. Эль-Гамаль');
  lines.push('');
  lines.push(...formatAlgorithmTable(elgResults));
  lines.push('');
  lines.push(`Среднее время шифрования Эль-Гамаля: ${avgElgEnc.toFixed(6)} ms`);
  lines.push(`Среднее время расшифрования Эль-Гамаля: ${avgElgDec.toFixed(6)} ms`);
  lines.push(`Средний рост объема Эль-Гамаля: x${avgElgRatio.toFixed(2)}`);
  lines.push('');
  lines.push('## 6. Сводное сравнение');
  lines.push('');
  lines.push(...formatComparisonTable(comparison));
  fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab8.txt'), lines.join('\n'), 'utf8');
}

async function main() {
  ensureDirs();
  const cli = parseArgs(process.argv.slice(2));
  const interactive = process.stdin.isTTY;
  const rl = interactive ? readline.createInterface({ input, output }) : null;
  try {
    console.log('ЛР №8: RSA и Эль-Гамаль.');
    const fio = interactive
      ? await ask(rl, 'Введите ФИО', cli.fio || DEFAULT_FIO)
      : (cli.fio || DEFAULT_FIO);
    const iterationsRaw = interactive
      ? await ask(rl, 'Итераций замера времени', cli.iterations || String(DEFAULT_ITERS))
      : (cli.iterations || String(DEFAULT_ITERS));
    const iterations = Number(iterationsRaw);
    if (!Number.isInteger(iterations) || iterations <= 0) {
      throw new Error('iterations должен быть положительным целым.');
    }

    const modexpRows = runModexpBenchmark();
    saveModexp(modexpRows);

    const { rsaResults, elgResults, comparison } = runAsymBench(fio, iterations);
    fs.writeFileSync(path.join(OUTPUT_DIR, 'rsa_results.json'), JSON.stringify(rsaResults, null, 2), 'utf8');
    fs.writeFileSync(path.join(OUTPUT_DIR, 'elgamal_results.json'), JSON.stringify(elgResults, null, 2), 'utf8');
    saveComparison(comparison);
    saveReport({ fio, iterations, modexpRows, rsaResults, elgResults, comparison });

    console.log('Готово. Сформированы файлы в output/.');
  } finally {
    if (rl) rl.close();
  }
}

main().catch((err) => {
  console.error('Ошибка:', err.message);
  process.exitCode = 1;
});
