#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const readline = require('readline/promises');
const { stdin: input, stdout: output } = require('process');

const DEFAULT_BBS_BITS = 256;
const DEFAULT_BBS_P = 383n;
const DEFAULT_BBS_Q = 503n;
const DEFAULT_BBS_X0 = 12345n;
const DEFAULT_RC4_N = 6;
const DEFAULT_RC4_KEY = [61, 60, 23, 22, 21, 20];
const DEFAULT_TEXT = 'Pavlovich Yan Andreevich';
const BLOCK_DIR = path.join(process.cwd(), 'output');
const ENV_PATH = path.join(process.cwd(), '.env');

function ensureOutputDir() {
  if (!fs.existsSync(BLOCK_DIR)) {
    fs.mkdirSync(BLOCK_DIR, { recursive: true });
  }
}

function ask(rl, question, fallback = '') {
  return rl.question(`${question}${fallback ? ` [${fallback}]` : ''}: `).then((ans) => {
    const trimmed = ans.trim();
    return trimmed || fallback;
  });
}

function parseEnvFile(filePath) {
  if (!fs.existsSync(filePath)) return {};
  const raw = fs.readFileSync(filePath, 'utf8');
  const out = {};
  for (const line of raw.split(/\r?\n/)) {
    const trimmed = line.trim();
    if (!trimmed || trimmed.startsWith('#')) continue;
    const eq = trimmed.indexOf('=');
    if (eq <= 0) continue;
    const key = trimmed.slice(0, eq).trim();
    let value = trimmed.slice(eq + 1).trim();
    if ((value.startsWith('"') && value.endsWith('"')) || (value.startsWith("'") && value.endsWith("'"))) {
      value = value.slice(1, -1);
    }
    out[key] = value;
  }
  return out;
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

function isPrime(n) {
  const x = BigInt(n);
  if (x < 2n) return false;
  if (x === 2n || x === 3n) return true;
  if (x % 2n === 0n || x % 3n === 0n) return false;
  for (let i = 5n; i * i <= x; i += 6n) {
    if (x % i === 0n || x % (i + 2n) === 0n) return false;
  }
  return true;
}

function modPow(base, exp, mod) {
  let b = BigInt(base) % BigInt(mod);
  let e = BigInt(exp);
  const m = BigInt(mod);
  let r = 1n;
  while (e > 0n) {
    if (e & 1n) r = (r * b) % m;
    b = (b * b) % m;
    e >>= 1n;
  }
  return r;
}

function parseBigIntInput(value, name) {
  const trimmed = String(value).trim();
  if (!/^[0-9]+$/.test(trimmed)) {
    throw new Error(`${name} должен быть положительным целым десятичным числом.`);
  }
  return BigInt(trimmed);
}

function parseDecimalList(value, expectedLen, name, maxValue = 255) {
  const items = value.split(',').map((s) => s.trim()).filter(Boolean);
  if (expectedLen && items.length !== expectedLen) {
    throw new Error(`${name} должен содержать ровно ${expectedLen} чисел через запятую.`);
  }
  const parsed = items.map((item) => {
    if (!/^[0-9]+$/.test(item)) {
      throw new Error(`${name} должен содержать только десятичные числа.`);
    }
    const n = Number(item);
    if (!Number.isInteger(n) || n < 0 || n > maxValue) {
      throw new Error(`${name} должен содержать значения в диапазоне 0..${maxValue}.`);
    }
    return n;
  });
  return parsed;
}

function toHex(buf) {
  return Buffer.from(buf).toString('hex').toUpperCase();
}

function normalizeRc4Word(value, nBits) {
  const max = (1 << nBits) - 1;
  if (!Number.isInteger(value) || value < 0 || value > max) {
    throw new Error(`Значение RC4 должно быть в диапазоне 0..${max} при n=${nBits}.`);
  }
  return value;
}

function validateBbsParams(p, q, x0) {
  const n = p * q;
  if (p === q) throw new Error('BBS: p и q должны различаться.');
  if (!isPrime(p) || !isPrime(q)) throw new Error('BBS: p и q должны быть простыми.');
  if (p % 4n !== 3n || q % 4n !== 3n) throw new Error('BBS: p и q должны быть сравнимы с 3 по модулю 4.');
  if (gcd(x0, n) !== 1n) throw new Error('BBS: x0 должен быть взаимно прост с n = p*q.');
  return n;
}

function loadConfigFromEnv() {
  const env = parseEnvFile(ENV_PATH);
  const bbsP = parseBigIntInput(env.BBS_P || DEFAULT_BBS_P.toString(), 'BBS_P');
  const bbsQ = parseBigIntInput(env.BBS_Q || DEFAULT_BBS_Q.toString(), 'BBS_Q');
  const bbsX0 = parseBigIntInput(env.BBS_X0 || DEFAULT_BBS_X0.toString(), 'BBS_X0');
  const rc4N = Number(env.RC4_N || DEFAULT_RC4_N);
  if (!Number.isInteger(rc4N) || rc4N < 1 || rc4N > 8) {
    throw new Error('RC4_N должен быть целым числом в диапазоне 1..8.');
  }
  const rc4Max = (1 << rc4N) - 1;
  const rc4KeyRaw = env.RC4_KEY_DECIMAL || DEFAULT_RC4_KEY.join(',');
  const rc4Key = parseDecimalList(rc4KeyRaw, 6, 'RC4_KEY_DECIMAL', rc4Max).map((v) => normalizeRc4Word(v, rc4N));
  const rc4Text = env.RC4_TEXT || DEFAULT_TEXT;

  const n = validateBbsParams(bbsP, bbsQ, bbsX0);
  return {
    bbs: { p: bbsP, q: bbsQ, x0: bbsX0, n },
    rc4: { n: rc4N, key: rc4Key, text: rc4Text }
  };
}

function bbsGenerateBits({ p, q, x0, bits }) {
  const n = p * q;
  let x = modPow(x0, 2n, n);
  const out = [];
  const steps = [];
  for (let i = 0; i < bits; i += 1) {
    x = modPow(x, 2n, n);
    const bit = Number(x & 1n);
    out.push(bit);
    steps.push({
      step: i + 1,
      x: x.toString(),
      bit
    });
  }
  return { n, seed: x0, finalState: x, bits: out, steps };
}

function bbsGenerateBytes(params, byteCount) {
  const bitCount = byteCount * 8;
  const run = bbsGenerateBits({ ...params, bits: bitCount });
  const bytes = [];
  for (let i = 0; i < run.bits.length; i += 8) {
    let byte = 0;
    for (let j = 0; j < 8; j += 1) {
      byte = (byte << 1) | run.bits[i + j];
    }
    bytes.push(byte);
  }
  return { ...run, bytes: Buffer.from(bytes) };
}

function rc4Init(keyBytes, nBits = 8) {
  const m = 1 << nBits;
  const S = Array.from({ length: m }, (_, i) => i);
  let j = 0;
  for (let i = 0; i < m; i += 1) {
    j = (j + S[i] + keyBytes[i % keyBytes.length]) % m;
    [S[i], S[j]] = [S[j], S[i]];
  }
  return S;
}

function rc4Keystream(keyBytes, length, nBits = 8) {
  const m = 1 << nBits;
  const S = rc4Init(keyBytes, nBits);
  let i = 0;
  let j = 0;
  const out = Buffer.alloc(length);
  for (let k = 0; k < length; k += 1) {
    i = (i + 1) % m;
    j = (j + S[i]) % m;
    [S[i], S[j]] = [S[j], S[i]];
    const t = (S[i] + S[j]) % m;
    out[k] = S[t];
  }
  return out;
}

function rc4Crypt(data, keyBytes, nBits = 8) {
  const inputBuf = Buffer.isBuffer(data) ? data : Buffer.from(data, 'utf8');
  const ks = rc4Keystream(keyBytes, inputBuf.length, nBits);
  const out = Buffer.alloc(inputBuf.length);
  for (let i = 0; i < inputBuf.length; i += 1) {
    out[i] = inputBuf[i] ^ ks[i];
  }
  return out;
}

function rc4InitWithTrace(keyBytes, nBits = 8) {
  const m = 1 << nBits;
  const S = Array.from({ length: m }, (_, i) => i);
  let j = 0;
  const trace = [];
  for (let i = 0; i < m; i += 1) {
    j = (j + S[i] + keyBytes[i % keyBytes.length]) % m;
    const beforeI = S[i];
    const beforeJ = S[j];
    [S[i], S[j]] = [S[j], S[i]];
    trace.push({
      i,
      j,
      keyByte: keyBytes[i % keyBytes.length],
      sI_before: beforeI,
      sJ_before: beforeJ,
      sI_after: S[i],
      sJ_after: S[j]
    });
  }
  return { S, trace };
}

function rc4KeystreamWithTrace(keyBytes, length, nBits = 8) {
  const m = 1 << nBits;
  const init = rc4InitWithTrace(keyBytes, nBits);
  const S = init.S;
  let i = 0;
  let j = 0;
  const out = Buffer.alloc(length);
  const trace = [];
  for (let k = 0; k < length; k += 1) {
    i = (i + 1) % m;
    j = (j + S[i]) % m;
    [S[i], S[j]] = [S[j], S[i]];
    const t = (S[i] + S[j]) % m;
    out[k] = S[t];
    trace.push({
      k,
      i,
      j,
      t,
      keyByte: out[k]
    });
  }
  return { stream: out, ksaTrace: init.trace, prgaTrace: trace };
}

function rc4CryptWithTrace(data, keyBytes, nBits = 8) {
  const inputBuf = Buffer.isBuffer(data) ? data : Buffer.from(data, 'utf8');
  const detailed = rc4KeystreamWithTrace(keyBytes, inputBuf.length, nBits);
  const out = Buffer.alloc(inputBuf.length);
  const xorTrace = [];
  for (let i = 0; i < inputBuf.length; i += 1) {
    out[i] = inputBuf[i] ^ detailed.stream[i];
    xorTrace.push({
      pos: i,
      plainDec: inputBuf[i],
      plainHex: inputBuf[i].toString(16).padStart(2, '0').toUpperCase(),
      streamDec: detailed.stream[i],
      streamHex: detailed.stream[i].toString(16).padStart(2, '0').toUpperCase(),
      cipherDec: out[i],
      cipherHex: out[i].toString(16).padStart(2, '0').toUpperCase()
    });
  }
  return {
    input: inputBuf,
    output: out,
    stream: detailed.stream,
    ksaTrace: detailed.ksaTrace,
    prgaTrace: detailed.prgaTrace,
    xorTrace
  };
}

function measureBbsSpeed(params, bits) {
  const t1 = process.hrtime.bigint();
  bbsGenerateBits({ ...params, bits });
  const t2 = process.hrtime.bigint();
  const ms = Number(t2 - t1) / 1e6;
  return {
    bits,
    milliseconds: ms,
    bitsPerSecond: bits / (ms / 1000),
    bytesPerSecond: (Math.floor(bits / 8) / (ms / 1000))
  };
}

function measureRc4Speed(keyBytes, text, nBits = 8) {
  const plain = Buffer.from(text, 'utf8');
  const t1 = process.hrtime.bigint();
  const cipher = rc4Crypt(plain, keyBytes, nBits);
  const t2 = process.hrtime.bigint();
  const decrypted = rc4Crypt(cipher, keyBytes, nBits);
  const t3 = process.hrtime.bigint();

  const streamOnlyT1 = process.hrtime.bigint();
  const stream = rc4Keystream(keyBytes, plain.length, nBits);
  const streamOnlyT2 = process.hrtime.bigint();

  const encryptMs = Number(t2 - t1) / 1e6;
  const decryptMs = Number(t3 - t2) / 1e6;
  const prgaMs = Number(streamOnlyT2 - streamOnlyT1) / 1e6;

  return {
    cipher,
    decrypted,
    encryptMs,
    decryptMs,
    prgaMs,
    encryptBytesPerSec: plain.length / (encryptMs / 1000),
    decryptBytesPerSec: plain.length / (decryptMs / 1000),
    prgaBytesPerSec: plain.length / (prgaMs / 1000),
    stream
  };
}

function rc4SelfTest() {
  const key = Buffer.from(DEFAULT_RC4_KEY);
  const sample = Buffer.from('RC4 self-test sample', 'utf8');
  const cipher = rc4Crypt(sample, key, 6);
  const decrypted = rc4Crypt(cipher, key, 6);
  if (!decrypted.equals(sample)) {
    throw new Error('Self-test RC4 failed: decrypt(encrypt(text)) != text');
  }
  const stream = rc4Keystream(key, 16, 6);
  if (stream.length !== 16) {
    throw new Error('Self-test RC4 failed: keystream length mismatch');
  }
}

function bbsSelfTest() {
  const p = 383n;
  const q = 503n;
  const x0 = 12345n;
  if (!isPrime(p) || !isPrime(q)) throw new Error('Self-test BBS failed: primes check');
  if (p % 4n !== 3n || q % 4n !== 3n) throw new Error('Self-test BBS failed: Blum primes check');
  if (gcd(x0, p * q) !== 1n) throw new Error('Self-test BBS failed: seed coprime check');
  const run = bbsGenerateBits({ p, q, x0, bits: 32 });
  if (run.bits.some((b) => b !== 0 && b !== 1)) {
    throw new Error('Self-test BBS failed: output is not binary');
  }
}

function renderTable(rows, headers) {
  const widths = headers.map((_, c) => Math.max(...[headers, ...rows].map((r) => String(r[c]).length)));
  const line = (row) => row.map((v, c) => String(v).padEnd(widths[c], ' ')).join(' | ');
  const sep = widths.map((w) => '-'.repeat(w)).join('-|-');
  return [line(headers), sep, ...rows.map(line)].join('\n');
}

function formatBits(bits) {
  return bits.join('');
}

function buildReport(data) {
  const bbsRows = data.bbs.bits.reduce((acc, bit, idx) => {
    if (idx % 8 === 0) acc.push([Math.floor(idx / 8), '']);
    const row = acc[acc.length - 1];
    row[1] += String(bit);
    return acc;
  }, []);
  const bbsStepsRows = data.bbs.steps.map((step) => [step.step, step.x, step.bit]);
  const rc4KsaRows = data.rc4.ksaTrace.map((row) => [
    row.i, row.j, row.keyByte, row.sI_before, row.sJ_before, row.sI_after, row.sJ_after
  ]);
  const rc4PrgaRows = data.rc4.prgaTrace.map((row) => [row.k, row.i, row.j, row.t, row.keyByte]);
  const rc4XorRows = data.rc4.xorTrace.map((row) => [
    row.pos, row.plainHex, row.streamHex, row.cipherHex
  ]);
  const fullBits = formatBits(data.bbs.bits);

  return [
    'lab 8',
    `Дата/время: ${new Date().toLocaleString()}`,
    '',
    '========== Приложение 1: BBS ==========', 
    `p = ${data.bbs.p.toString()}`,
    `q = ${data.bbs.q.toString()}`,
    `Модуль N = ${data.bbs.n.toString()}`,
    `seed x0 = ${data.bbs.x0.toString()}`,
    `gcd(x0, n) = ${data.bbs.gcd.toString()}`,
    `Длина ПСП n = ${data.bbs.bits.length} бит`,
    `Скорость: ${data.bbs.speed.bitsPerSecond.toFixed(2)} бит/с`,
    `Все ${data.bbs.bits.length} бит: ${fullBits}`,
    '',
    'BBS: последовательность в блоках по 8 бит (все блоки):',
    renderTable(bbsRows, ['Блок', 'Биты']),
    '',
    'BBS: пошаговая трассировка (каждый шаг генерации):',
    renderTable(bbsStepsRows, ['Шаг', 'x_i', 'b_i = x_i mod 2']),
    '',
    '========== Приложение 2: RC4 ==========', 
    `n = ${data.rc4.n}`,
    `Ключ (decimal): ${data.rc4.key.join(', ')}`,
    `Ключ (hex): ${toHex(Buffer.from(data.rc4.key))}`,
    `Текст: ${data.rc4.text}`,
    `Текст (hex): ${toHex(Buffer.from(data.rc4.text, 'utf8'))}`,
    `Поток ключа (decimal): ${Array.from(data.rc4.stream).join(', ')}`,
    `Поток ключа (hex): ${toHex(data.rc4.stream)}`,
    `Шифртекст (hex): ${toHex(data.rc4.cipher)}`,
    `Расшифрование совпало: ${data.rc4.decrypted.equals(Buffer.from(data.rc4.text, 'utf8')) ? 'ДА' : 'НЕТ'}`,
    `Скорость PRGA: ${data.rc4.speed.prgaBytesPerSec.toFixed(2)} байт/с`,
    `Скорость шифрования: ${data.rc4.speed.encryptBytesPerSec.toFixed(2)} байт/с`,
    `Скорость расшифрования: ${data.rc4.speed.decryptBytesPerSec.toFixed(2)} байт/с`,
    '',
    'RC4: KSA трассировка (перемешивание S):',
    renderTable(rc4KsaRows, ['i', 'j', 'key[i mod L]', 'S[i] до', 'S[j] до', 'S[i] после', 'S[j] после']),
    '',
    'RC4: PRGA трассировка (генерация потока):',
    renderTable(rc4PrgaRows, ['k', 'i', 'j', 't', 'keyStream[k]']),
    '',
    'RC4: XOR по байтам (шифрование):',
    renderTable(rc4XorRows, ['pos', 'plain(hex)', 'stream(hex)', 'cipher(hex)']),
    ''
  ].join('\n');
}

async function runBbsApp(params, bitsToGenerate) {
  const speed = measureBbsSpeed(params, bitsToGenerate);
  const byteCount = Math.ceil(bitsToGenerate / 8);
  const bytesRun = bbsGenerateBytes(params, byteCount);
  const bits = bytesRun.bits.slice(0, bitsToGenerate);
  const steps = bytesRun.steps.slice(0, bitsToGenerate);

  return {
    p: params.p,
    q: params.q,
    x0: params.x0,
    n: params.n,
    gcd: gcd(params.x0, params.n),
    bits,
    steps,
    speed,
    bytesHex: toHex(bytesRun.bytes),
    finalState: bytesRun.finalState,
    bitsToGenerate
  };
}

function runRc4App(params) {
  const keyBytes = Buffer.from(params.key);
  const speed = measureRc4Speed(keyBytes, params.text, params.n);
  const detailed = rc4CryptWithTrace(params.text, keyBytes, params.n);
  return {
    n: params.n,
    key: params.key,
    text: params.text,
    cipher: detailed.output,
    decrypted: speed.decrypted,
    speed,
    stream: detailed.stream,
    ksaTrace: detailed.ksaTrace,
    prgaTrace: detailed.prgaTrace,
    xorTrace: detailed.xorTrace
  };
}

function writeArtifacts(data) {
  ensureOutputDir();
  const reportPath = path.join(BLOCK_DIR, 'report_lab8.txt');
  const jsonPath = path.join(BLOCK_DIR, 'results.json');
  const rc4CipherPath = path.join(BLOCK_DIR, 'rc4_cipher.bin');
  const rc4PlainPath = path.join(BLOCK_DIR, 'rc4_decrypted.txt');
  const legacyBbsBitsPath = path.join(BLOCK_DIR, 'bbs_bits.txt');
  const legacyBbsBytesPath = path.join(BLOCK_DIR, 'bbs_bytes.hex');

  fs.writeFileSync(reportPath, buildReport(data), 'utf8');
  const jsonReplacer = (_, value) => (typeof value === 'bigint' ? value.toString() : value);
  fs.writeFileSync(jsonPath, JSON.stringify({
    timestamp: new Date().toISOString(),
    bbs: {
      p: data.bbs.p.toString(),
      q: data.bbs.q.toString(),
      x0: data.bbs.x0.toString(),
      n: data.bbs.n.toString(),
      gcd: data.bbs.gcd.toString(),
      bitsToGenerate: data.bbs.bitsToGenerate,
      bitsAll: data.bbs.bits.join(''),
      bitsPreview64: data.bbs.bits.slice(0, 64).join(''),
      bytesHexPreview16: data.bbs.bytesHex.slice(0, 32),
      speed: {
        milliseconds: data.bbs.speed.milliseconds,
        bitsPerSecond: data.bbs.speed.bitsPerSecond,
        bytesPerSecond: data.bbs.speed.bytesPerSecond
      }
    },
    rc4: {
      n: data.rc4.n,
      key: data.rc4.key,
      text: data.rc4.text,
      cipherHex: toHex(data.rc4.cipher),
      decrypted: data.rc4.decrypted.toString('utf8'),
      speed: {
        encryptMs: data.rc4.speed.encryptMs,
        decryptMs: data.rc4.speed.decryptMs,
        prgaMs: data.rc4.speed.prgaMs,
        encryptBytesPerSec: data.rc4.speed.encryptBytesPerSec,
        decryptBytesPerSec: data.rc4.speed.decryptBytesPerSec,
        prgaBytesPerSec: data.rc4.speed.prgaBytesPerSec
      }
    }
  }, jsonReplacer, 2), 'utf8');
  fs.writeFileSync(rc4CipherPath, data.rc4.cipher);
  fs.writeFileSync(rc4PlainPath, data.rc4.decrypted.toString('utf8'), 'utf8');
  if (fs.existsSync(legacyBbsBitsPath)) fs.unlinkSync(legacyBbsBitsPath);
  if (fs.existsSync(legacyBbsBytesPath)) fs.unlinkSync(legacyBbsBytesPath);

  return { reportPath, jsonPath, rc4CipherPath, rc4PlainPath };
}

function runSelfTests() {
  rc4SelfTest();
  bbsSelfTest();
}

async function main() {
  runSelfTests();
  const rl = readline.createInterface({ input, output });

  try {
    const config = loadConfigFromEnv();
    console.log('=== Lab 8 ===');
    const bitsAnswer = await ask(rl, 'Сколько бит сгенерировать', String(DEFAULT_BBS_BITS));
    const bitsToGenerate = parseInt(bitsAnswer, 10) || DEFAULT_BBS_BITS;
    if (bitsToGenerate <= 0) {
      throw new Error('Количество бит должно быть положительным целым числом.');
    }
    const data = {
      bbs: await runBbsApp(config.bbs, bitsToGenerate),
      rc4: runRc4App(config.rc4)
    };

    const artifacts = writeArtifacts(data);

    console.log('\n--- Краткий итог ---');
    console.log(`BBS: N=${data.bbs.n.toString()}, length=${data.bbs.bits.length} бит, speed=${data.bbs.speed.bitsPerSecond.toFixed(2)} бит/с`);
    console.log(`RC4: key=${data.rc4.key.join(', ')}, cipher=${toHex(data.rc4.cipher)}, ok=${data.rc4.decrypted.equals(Buffer.from(data.rc4.text, 'utf8')) ? 'ДА' : 'НЕТ'}`);
    console.log(`Параметры загружены из: ${ENV_PATH}`);
    console.log(`Отчет: ${artifacts.reportPath}`);
    console.log(`JSON:   ${artifacts.jsonPath}`);
    console.log(`RC4 bin:  ${artifacts.rc4CipherPath}`);
    console.log(`RC4 text: ${artifacts.rc4PlainPath}`);
  } catch (error) {
    console.error(`Ошибка: ${error.message}`);
    process.exitCode = 1;
  } finally {
    rl.close();
  }
}

main();
