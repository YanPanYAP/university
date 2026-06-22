'use strict';

const crypto = require('crypto');
const fs = require('fs');
const path = require('path');

const ALGORITHMS = new Map([
  ['md5', { label: 'MD5', bits: 128, maxInput: '2^64 - 1 бит', blockBits: 512 }],
  ['sha1', { label: 'SHA-1', bits: 160, maxInput: '2^64 - 1 бит', blockBits: 512 }],
  ['sha256', { label: 'SHA-256', bits: 256, maxInput: '2^64 - 1 бит', blockBits: 512 }],
  ['sha512', { label: 'SHA-512', bits: 512, maxInput: '2^128 - 1 бит', blockBits: 1024 }]
]);

const DEFAULT_MESSAGE = 'Pavlovich Yan Andreevich';
const DEFAULT_SIZES = [0, 16, 64, 256, 1024, 4096, 16384, 65536, 262144, 1048576];
const DEFAULT_ITERATIONS = 1000;

function normalizeAlgorithm(algorithm = 'sha256') {
  const key = String(algorithm).toLowerCase();
  if (!ALGORITHMS.has(key)) {
    throw new Error(`Неподдерживаемый алгоритм: ${algorithm}`);
  }
  return key;
}

function getAlgorithmInfo(algorithm = 'sha256') {
  const key = normalizeAlgorithm(algorithm);
  return { id: key, ...ALGORITHMS.get(key) };
}

function computeHash(message, algorithm = 'sha256', inputEncoding = 'utf8') {
  const key = normalizeAlgorithm(algorithm);
  return crypto.createHash(key).update(String(message), inputEncoding).digest('hex');
}

function hexToBuffer(hex) {
  return Buffer.from(hex, 'hex');
}

function popcountByte(byte) {
  let n = byte;
  let count = 0;
  while (n > 0) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}

function bitDifference(leftHex, rightHex) {
  const left = hexToBuffer(leftHex);
  const right = hexToBuffer(rightHex);
  if (left.length !== right.length) {
    throw new Error('Хеши должны иметь одинаковую длину.');
  }
  let diff = 0;
  for (let i = 0; i < left.length; i += 1) {
    diff += popcountByte(left[i] ^ right[i]);
  }
  return diff;
}

function formatScientific(value, digits = 3) {
  if (value === 0) return '0';
  const exponent = Math.floor(Math.log10(value));
  const mantissa = value / (10 ** exponent);
  return `${mantissa.toFixed(digits)} * 10^${exponent}`;
}

function birthdayCollisionWork(hashBits, probability = 0.99) {
  const bits = Number(hashBits);
  const p = Number(probability);
  if (!Number.isFinite(bits) || bits <= 0) throw new Error('hashBits должен быть положительным числом.');
  if (!Number.isFinite(p) || p <= 0 || p >= 1) throw new Error('probability должна быть в диапазоне (0, 1).');
  const lnNoCollision = Math.log(1 / (1 - p));
  const log10Operations = 0.5 * (Math.log10(2 * lnNoCollision) + bits * Math.log10(2));
  const exponent = Math.floor(log10Operations);
  const mantissa = 10 ** (log10Operations - exponent);
  return {
    hashBits: bits,
    probability: p,
    outputSpace: `2^${bits}`,
    operationsApprox: `sqrt(2 * 2^${bits} * ln(${(1 / (1 - p)).toFixed(0)}))`,
    operationsScientific: `${mantissa.toFixed(3)} * 10^${exponent}`
  };
}

function aggregate(values) {
  const sum = values.reduce((acc, value) => acc + value, 0);
  return {
    meanMs: sum / values.length,
    minMs: Math.min(...values),
    maxMs: Math.max(...values)
  };
}

function makeInput(size) {
  const pattern = 'HashLab9-SHA256-Pavlovich-';
  return Buffer.from(pattern.repeat(Math.ceil(size / pattern.length)).slice(0, size), 'utf8');
}

function benchmarkHash({ algorithm = 'sha256', sizes = DEFAULT_SIZES, iterations = DEFAULT_ITERATIONS } = {}) {
  const key = normalizeAlgorithm(algorithm);
  const runs = Number(iterations);
  if (!Number.isInteger(runs) || runs <= 0) throw new Error('iterations должен быть положительным целым.');
  return sizes.map((size) => {
    const inputBytes = Number(size);
    const input = makeInput(inputBytes);
    const times = [];
    let digest = '';
    for (let i = 0; i < runs; i += 1) {
      const t1 = process.hrtime.bigint();
      digest = crypto.createHash(key).update(input).digest('hex');
      const t2 = process.hrtime.bigint();
      times.push(Number(t2 - t1) / 1e6);
    }
    const stats = aggregate(times);
    const seconds = stats.meanMs / 1000;
    const mb = inputBytes / (1024 * 1024);
    return {
      inputBytes,
      iterations: runs,
      meanMs: stats.meanMs,
      minMs: stats.minMs,
      maxMs: stats.maxMs,
      mbPerSec: seconds === 0 ? 0 : mb / seconds,
      digestHexLength: digest.length
    };
  });
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
  const separator = widths.map((width, col) => (
    alignments[col] === 'right' ? `${'-'.repeat(width - 1)}:` : '-'.repeat(width)
  ));
  const line = (cells) => `| ${cells.map(padCell).join(' | ')} |`;
  return [line(headers), line(separator), ...rows.map(line)];
}

function buildBenchmarkTable(rows) {
  return formatTable(
    ['Input bytes', 'Iterations', 'Mean ms', 'Min ms', 'Max ms', 'MB/s', 'Hash hex chars'],
    rows.map((row) => [
      row.inputBytes,
      row.iterations,
      row.meanMs.toFixed(6),
      row.minMs.toFixed(6),
      row.maxMs.toFixed(6),
      row.mbPerSec.toFixed(2),
      row.digestHexLength
    ]),
    ['right', 'right', 'right', 'right', 'right', 'right', 'right']
  );
}

function readEncryptionResults() {
  const filePath = path.join(__dirname, '..', '..', 'lab 9', 'output', 'results.json');
  if (!fs.existsSync(filePath)) return null;
  return JSON.parse(fs.readFileSync(filePath, 'utf8'));
}

function byteToBits(char) {
  return char.charCodeAt(0).toString(2).padStart(8, '0');
}

function decryptBasketSum(cipherValue, key) {
  return (BigInt(cipherValue) * BigInt(key.aInv)) % BigInt(key.n);
}

function greedyBits(sumValue, privateKey) {
  let rest = BigInt(sumValue);
  const bits = Array(privateKey.length).fill('0');
  for (let i = privateKey.length - 1; i >= 0; i -= 1) {
    const weight = BigInt(privateKey[i]);
    if (weight <= rest) {
      bits[i] = '1';
      rest -= weight;
    }
  }
  return bits.join('');
}

function buildEncryptionSection(message) {
  const results = readEncryptionResults();
  if (!results || results.fio !== String(message)) return [];

  const ascii = results.mainExperiment.ascii;
  const rows = [...String(message)].map((char, index) => {
    const bits = byteToBits(char);
    const cipher = ascii.cipher[index];
    const sum = decryptBasketSum(cipher, ascii);
    const restoredBits = greedyBits(sum, ascii.d);
    const restoredChar = String.fromCharCode(parseInt(restoredBits, 2));
    return [
      index + 1,
      char === ' ' ? '<space>' : char,
      bits,
      cipher,
      sum.toString(),
      restoredBits,
      restoredChar === ' ' ? '<space>' : restoredChar
    ];
  });

  return [
    '2. Шифрование/расшифрование',
    `M = ${message}`,
    `d = (${ascii.d.join(', ')})`,
    `e = (${ascii.e.join(', ')})`,
    `n = ${ascii.n}`,
    `a = ${ascii.a}`,
    `a^-1 mod n = ${ascii.aInv}`,
    '',
    'Шифрование:',
    'C_i = сумма e_j, где в блоке M_i на позиции j стоит 1',
    `C = ${ascii.cipher.join(' ')}`,
    '',
    'Расшифрование:',
    'S_i = C_i * a^-1 mod n',
    ...formatTable(
      ['#', 'M_i', 'bits', 'C_i', 'S_i', 'bits restored', 'M_i restored'],
      rows,
      ['right', 'left', 'left', 'right', 'right', 'left', 'left']
    ),
    '',
    `M восстановленное = ${ascii.decrypted}`
  ];
}

function buildReport({ algorithm = 'sha256', message = DEFAULT_MESSAGE, benchmarkRows } = {}) {
  const info = getAlgorithmInfo(algorithm);
  const msg = String(message);
  const hash = computeHash(msg, info.id);
  const changedMessage = msg.length > 0
    ? `${msg[0] === msg[0].toUpperCase() ? msg[0].toLowerCase() : msg[0].toUpperCase()}${msg.slice(1)}`
    : 'changed';
  const changedHash = computeHash(changedMessage, info.id);
  const diffBits = bitDifference(hash, changedHash);
  const birthday = birthdayCollisionWork(info.bits, 0.99);
  const rows = benchmarkRows || benchmarkHash({ algorithm: info.id });

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`Алгоритм: ${info.label}`);
  lines.push(`Длина хеша: ${info.bits} бит`);
  lines.push(`Размер блока: ${info.blockBits} бит`);
  lines.push(`Максимальная длина входного сообщения: ${info.maxInput}`);
  lines.push(`Тестовое сообщение: ${msg}`);
  lines.push('');
  lines.push(...buildEncryptionSection(msg));
  lines.push('');
  lines.push('3. Хеш сообщения');
  lines.push(`H(M) = ${hash}`);
  lines.push(`M' = ${changedMessage}`);
  lines.push(`H(M') = ${changedHash}`);
  lines.push(`Отличающихся битов между H(M) и H(M'): ${diffBits} из ${info.bits}`);
  lines.push('');
  lines.push('4. Парадокс дня рождения');
  lines.push(`Пространство хешей: ${birthday.outputSpace}`);
  lines.push(`Оценка числа сообщений для коллизии с вероятностью ${birthday.probability}: ${birthday.operationsApprox}`);
  lines.push(`Приближенно: ${birthday.operationsScientific}`);
  lines.push('');
  lines.push('5. Быстродействие');
  lines.push(...buildBenchmarkTable(rows));
  return lines.join('\n');
}

module.exports = {
  ALGORITHMS,
  DEFAULT_MESSAGE,
  DEFAULT_SIZES,
  DEFAULT_ITERATIONS,
  getAlgorithmInfo,
  computeHash,
  bitDifference,
  birthdayCollisionWork,
  benchmarkHash,
  buildEncryptionSection,
  buildBenchmarkTable,
  buildReport,
  formatScientific
};
