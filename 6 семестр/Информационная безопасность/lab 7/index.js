#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const zlib = require('zlib');
const crypto = require('crypto');
const readline = require('readline/promises');
const { stdin: input, stdout: output } = require('process');

const BLOCK_SIZE = 8;

const IP = [
  58, 50, 42, 34, 26, 18, 10, 2,
  60, 52, 44, 36, 28, 20, 12, 4,
  62, 54, 46, 38, 30, 22, 14, 6,
  64, 56, 48, 40, 32, 24, 16, 8,
  57, 49, 41, 33, 25, 17, 9, 1,
  59, 51, 43, 35, 27, 19, 11, 3,
  61, 53, 45, 37, 29, 21, 13, 5,
  63, 55, 47, 39, 31, 23, 15, 7
];

const FP = [
  40, 8, 48, 16, 56, 24, 64, 32,
  39, 7, 47, 15, 55, 23, 63, 31,
  38, 6, 46, 14, 54, 22, 62, 30,
  37, 5, 45, 13, 53, 21, 61, 29,
  36, 4, 44, 12, 52, 20, 60, 28,
  35, 3, 43, 11, 51, 19, 59, 27,
  34, 2, 42, 10, 50, 18, 58, 26,
  33, 1, 41, 9, 49, 17, 57, 25
];

const E = [
  32, 1, 2, 3, 4, 5,
  4, 5, 6, 7, 8, 9,
  8, 9, 10, 11, 12, 13,
  12, 13, 14, 15, 16, 17,
  16, 17, 18, 19, 20, 21,
  20, 21, 22, 23, 24, 25,
  24, 25, 26, 27, 28, 29,
  28, 29, 30, 31, 32, 1
];

const P = [
  16, 7, 20, 21,
  29, 12, 28, 17,
  1, 15, 23, 26,
  5, 18, 31, 10,
  2, 8, 24, 14,
  32, 27, 3, 9,
  19, 13, 30, 6,
  22, 11, 4, 25
];

const PC1 = [
  57, 49, 41, 33, 25, 17, 9,
  1, 58, 50, 42, 34, 26, 18,
  10, 2, 59, 51, 43, 35, 27,
  19, 11, 3, 60, 52, 44, 36,
  63, 55, 47, 39, 31, 23, 15,
  7, 62, 54, 46, 38, 30, 22,
  14, 6, 61, 53, 45, 37, 29,
  21, 13, 5, 28, 20, 12, 4
];

const PC2 = [
  14, 17, 11, 24, 1, 5,
  3, 28, 15, 6, 21, 10,
  23, 19, 12, 4, 26, 8,
  16, 7, 27, 20, 13, 2,
  41, 52, 31, 37, 47, 55,
  30, 40, 51, 45, 33, 48,
  44, 49, 39, 56, 34, 53,
  46, 42, 50, 36, 29, 32
];

const SHIFTS = [1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1];

const SBOX = [
  [
    [14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],
    [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8],
    [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0],
    [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]
  ],
  [
    [15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10],
    [3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5],
    [0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15],
    [13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9]
  ],
  [
    [10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8],
    [13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1],
    [13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7],
    [1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12]
  ],
  [
    [7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15],
    [13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9],
    [10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4],
    [3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14]
  ],
  [
    [2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
    [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
    [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
    [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]
  ],
  [
    [12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11],
    [10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8],
    [9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6],
    [4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13]
  ],
  [
    [4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1],
    [13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6],
    [1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2],
    [6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12]
  ],
  [
    [13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7],
    [1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2],
    [7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8],
    [2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11]
  ]
];

const WEAK_KEYS = [
  '0101010101010101',
  '1F1F1F1F0E0E0E0E',
  'E0E0E0E0F1F1F1F1',
  'FEFEFEFEFEFEFEFE'
];

const SEMI_WEAK_PAIRS = [
  ['01FE01FE01FE01FE', 'FE01FE01FE01FE01'],
  ['1FE01FE00EF10EF1', 'E01FE01FF10EF10E'],
  ['01E001E001F101F1', 'E001E001F101F101'],
  ['1FFE1FFE0EFE0EFE', 'FE1FFE1FFE0EFE0E'],
  ['011F011F010E010E', '1F011F010E010E01'],
  ['E0FEE0FEF1FEF1FE', 'FEE0FEE0FEF1FEF1']
];

function ask(rl, question, fallback = '') {
  return rl.question(`${question}${fallback ? ` [${fallback}]` : ''}: `).then((answer) => {
    const trimmed = answer.trim();
    return trimmed || fallback;
  });
}

function cleanHex(hex) {
  return hex.replace(/[^0-9a-fA-F]/g, '').toUpperCase();
}

function parseKeyHex(hex) {
  const normalized = cleanHex(hex);
  if (normalized.length !== 16) {
    throw new Error('Ключ должен содержать ровно 16 hex-символов (8 байт).');
  }
  return Buffer.from(normalized, 'hex');
}

function toBits(buf) {
  const bits = [];
  for (const byte of buf) {
    for (let i = 7; i >= 0; i -= 1) {
      bits.push((byte >> i) & 1);
    }
  }
  return bits;
}

function fromBits(bits) {
  const bytes = [];
  for (let i = 0; i < bits.length; i += 8) {
    let byte = 0;
    for (let j = 0; j < 8; j += 1) {
      byte = (byte << 1) | bits[i + j];
    }
    bytes.push(byte);
  }
  return Buffer.from(bytes);
}

function permute(bits, table) {
  return table.map((index) => bits[index - 1]);
}

function xorBits(a, b) {
  return a.map((bit, i) => bit ^ b[i]);
}

function rotateLeft(bits, count) {
  return bits.slice(count).concat(bits.slice(0, count));
}

function generateSubkeys(keyBytes) {
  const keyBits = toBits(keyBytes);
  const pc1 = permute(keyBits, PC1);
  let c = pc1.slice(0, 28);
  let d = pc1.slice(28, 56);
  const subkeys = [];

  for (const shift of SHIFTS) {
    c = rotateLeft(c, shift);
    d = rotateLeft(d, shift);
    subkeys.push(permute(c.concat(d), PC2));
  }

  return subkeys;
}

function sboxTransform(bits48) {
  const out = [];
  for (let i = 0; i < 8; i += 1) {
    const chunk = bits48.slice(i * 6, (i + 1) * 6);
    const row = (chunk[0] << 1) | chunk[5];
    const col = (chunk[1] << 3) | (chunk[2] << 2) | (chunk[3] << 1) | chunk[4];
    const value = SBOX[i][row][col];
    out.push((value >> 3) & 1, (value >> 2) & 1, (value >> 1) & 1, value & 1);
  }
  return out;
}

function feistel(right32, subkey48) {
  const expanded = permute(right32, E);
  const mixed = xorBits(expanded, subkey48);
  const substituted = sboxTransform(mixed);
  return permute(substituted, P);
}

function desBlock(input8, key8, mode) {
  const subkeys = generateSubkeys(key8);
  const subkeyOrder = mode === 'encrypt' ? subkeys : [...subkeys].reverse();

  let bits = permute(toBits(input8), IP);
  let left = bits.slice(0, 32);
  let right = bits.slice(32, 64);

  for (let round = 0; round < 16; round += 1) {
    const nextLeft = right;
    const f = feistel(right, subkeyOrder[round]);
    const nextRight = xorBits(left, f);
    left = nextLeft;
    right = nextRight;
  }

  bits = permute(right.concat(left), FP);
  return fromBits(bits);
}

function padPkcs7(buf) {
  const pad = BLOCK_SIZE - (buf.length % BLOCK_SIZE || BLOCK_SIZE);
  return Buffer.concat([buf, Buffer.alloc(pad || BLOCK_SIZE, pad || BLOCK_SIZE)]);
}

function unpadPkcs7(buf) {
  if (buf.length === 0 || buf.length % BLOCK_SIZE !== 0) {
    throw new Error('Некорректная длина буфера для PKCS7.');
  }
  const pad = buf[buf.length - 1];
  if (pad < 1 || pad > BLOCK_SIZE) {
    throw new Error('Некорректный PKCS7 padding (длина).');
  }
  for (let i = buf.length - pad; i < buf.length; i += 1) {
    if (buf[i] !== pad) {
      throw new Error('Некорректный PKCS7 padding (содержимое).');
    }
  }
  return buf.slice(0, -pad);
}

function splitBlocks(buf) {
  const blocks = [];
  for (let i = 0; i < buf.length; i += BLOCK_SIZE) {
    blocks.push(buf.slice(i, i + BLOCK_SIZE));
  }
  return blocks;
}

function desEee2Encrypt(plain, k1, k2) {
  const padded = padPkcs7(plain);
  const out = Buffer.alloc(padded.length);

  for (let i = 0; i < padded.length; i += BLOCK_SIZE) {
    const b = padded.slice(i, i + BLOCK_SIZE);
    const s1 = desBlock(b, k1, 'encrypt');
    const s2 = desBlock(s1, k2, 'encrypt');
    const s3 = desBlock(s2, k1, 'encrypt');
    s3.copy(out, i);
  }

  return out;
}

function desEee2Decrypt(cipher, k1, k2) {
  if (cipher.length % BLOCK_SIZE !== 0) {
    throw new Error('Длина шифртекста должна быть кратна 8 байтам.');
  }

  const out = Buffer.alloc(cipher.length);
  for (let i = 0; i < cipher.length; i += BLOCK_SIZE) {
    const b = cipher.slice(i, i + BLOCK_SIZE);
    const s1 = desBlock(b, k1, 'decrypt');
    const s2 = desBlock(s1, k2, 'decrypt');
    const s3 = desBlock(s2, k1, 'decrypt');
    s3.copy(out, i);
  }

  return unpadPkcs7(out);
}

function measureSpeed(plain, k1, k2, iterations) {
  const payload = Buffer.from(plain);

  const t1 = process.hrtime.bigint();
  for (let i = 0; i < iterations; i += 1) {
    desEee2Encrypt(payload, k1, k2);
  }
  const t2 = process.hrtime.bigint();

  const encrypted = desEee2Encrypt(payload, k1, k2);
  const t3 = process.hrtime.bigint();
  for (let i = 0; i < iterations; i += 1) {
    desEee2Decrypt(encrypted, k1, k2);
  }
  const t4 = process.hrtime.bigint();

  const encryptMs = Number(t2 - t1) / 1e6;
  const decryptMs = Number(t4 - t3) / 1e6;

  return {
    iterations,
    payloadBytes: payload.length,
    encryptMs,
    decryptMs,
    encryptPerSec: (iterations * payload.length) / (encryptMs / 1000),
    decryptPerSec: (iterations * payload.length) / (decryptMs / 1000)
  };
}

function bitDifferenceCount(a, b) {
  let diff = 0;
  for (let i = 0; i < a.length; i += 1) {
    let x = a[i] ^ b[i];
    while (x) {
      x &= x - 1;
      diff += 1;
    }
  }
  return diff;
}

function charDifferenceCountHex(a, b) {
  const ah = a.toString('hex');
  const bh = b.toString('hex');
  let diff = 0;
  for (let i = 0; i < ah.length; i += 1) {
    if (ah[i] !== bh[i]) diff += 1;
  }
  return diff;
}

function analyzeAvalancheByPlainBit(word, k1, k2) {
  const normalized = Buffer.alloc(BLOCK_SIZE, 0);
  Buffer.from(word, 'utf8').copy(normalized, 0, 0, BLOCK_SIZE);

  const baseCipher = desEee2Encrypt(normalized, k1, k2).slice(0, BLOCK_SIZE);
  const steps = [];

  for (let bitPos = 0; bitPos < 64; bitPos += 1) {
    const changed = Buffer.from(normalized);
    const byteIndex = Math.floor(bitPos / 8);
    const bitInByte = 7 - (bitPos % 8);
    changed[byteIndex] ^= (1 << bitInByte);

    const cipher = desEee2Encrypt(changed, k1, k2).slice(0, BLOCK_SIZE);
    const bitDiff = bitDifferenceCount(baseCipher, cipher);
    const charDiff = charDifferenceCountHex(baseCipher, cipher);

    steps.push({
      bitPos,
      modifiedBlockHex: changed.toString('hex').toUpperCase(),
      cipherHex: cipher.toString('hex').toUpperCase(),
      changedBits: bitDiff,
      changedHexChars: charDiff
    });
  }

  const avgBits = steps.reduce((sum, s) => sum + s.changedBits, 0) / steps.length;
  const avgHexChars = steps.reduce((sum, s) => sum + s.changedHexChars, 0) / steps.length;

  return {
    sourceBlockHex: normalized.toString('hex').toUpperCase(),
    baseCipherHex: baseCipher.toString('hex').toUpperCase(),
    averageChangedBits: avgBits,
    averageChangedHexChars: avgHexChars,
    steps
  };
}

function analyzeWeakAndSemiWeak(sample, strongK1, strongK2) {
  const sampleBlock = Buffer.alloc(BLOCK_SIZE, 0);
  Buffer.from(sample, 'utf8').copy(sampleBlock, 0, 0, BLOCK_SIZE);

  const normalCipher = desEee2Encrypt(sampleBlock, strongK1, strongK2).slice(0, BLOCK_SIZE);
  const normalAvalanche = analyzeAvalancheByPlainBit(sample, strongK1, strongK2);

  const weakResults = WEAK_KEYS.map((keyHex) => {
    const k = Buffer.from(keyHex, 'hex');
    const c = desEee2Encrypt(sampleBlock, k, k).slice(0, BLOCK_SIZE);
    const av = analyzeAvalancheByPlainBit(sample, k, k);
    return {
      key: keyHex,
      cipherHex: c.toString('hex').toUpperCase(),
      avgChangedBits: av.averageChangedBits,
      avgChangedHexChars: av.averageChangedHexChars
    };
  });

  const semiWeakResults = SEMI_WEAK_PAIRS.map(([k1h, k2h]) => {
    const k1 = Buffer.from(k1h, 'hex');
    const k2 = Buffer.from(k2h, 'hex');
    const c = desEee2Encrypt(sampleBlock, k1, k2).slice(0, BLOCK_SIZE);
    const av = analyzeAvalancheByPlainBit(sample, k1, k2);
    return {
      pair: `${k1h}/${k2h}`,
      cipherHex: c.toString('hex').toUpperCase(),
      avgChangedBits: av.averageChangedBits,
      avgChangedHexChars: av.averageChangedHexChars
    };
  });

  return {
    baseline: {
      keys: `${strongK1.toString('hex').toUpperCase()}/${strongK2.toString('hex').toUpperCase()}`,
      cipherHex: normalCipher.toString('hex').toUpperCase(),
      avgChangedBits: normalAvalanche.averageChangedBits,
      avgChangedHexChars: normalAvalanche.averageChangedHexChars
    },
    weak: weakResults,
    semiWeak: semiWeakResults
  };
}

function compressionStats(plain, cipher) {
  const gzPlain = zlib.gzipSync(plain, { level: 9 });
  const gzCipher = zlib.gzipSync(cipher, { level: 9 });

  const plainRatio = (gzPlain.length / plain.length) * 100;
  const cipherRatio = (gzCipher.length / cipher.length) * 100;

  return {
    plainBytes: plain.length,
    cipherBytes: cipher.length,
    gzPlainBytes: gzPlain.length,
    gzCipherBytes: gzCipher.length,
    plainCompressionPercent: 100 - plainRatio,
    cipherCompressionPercent: 100 - cipherRatio,
    plainRatioPercent: plainRatio,
    cipherRatioPercent: cipherRatio
  };
}

function runSelfTests() {
  const desKey = Buffer.from('133457799BBCDFF1', 'hex');
  const desPlain = Buffer.from('0123456789ABCDEF', 'hex');
  const expectedCipher = '85E813540F0AB405';
  const actualCipher = desBlock(desPlain, desKey, 'encrypt').toString('hex').toUpperCase();
  if (actualCipher !== expectedCipher) {
    throw new Error(`Self-test DES провален: ожидалось ${expectedCipher}, получено ${actualCipher}`);
  }

  const k1 = Buffer.from('133457799BBCDFF1', 'hex');
  const k2 = Buffer.from('1F1F1F1F0E0E0E0E', 'hex');
  const msg = Buffer.from('DES-EEE2 self-check', 'utf8');
  const enc = desEee2Encrypt(msg, k1, k2);
  const dec = desEee2Decrypt(enc, k1, k2);
  if (!dec.equals(msg)) {
    throw new Error('Self-test DES-EEE2 провален: расшифрование не совпало с исходным текстом.');
  }
}

function ensureOutputDir() {
  const outputDir = path.join(process.cwd(), 'output');
  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir, { recursive: true });
  }
  return outputDir;
}

function formatTable(rows, headers) {
  const allRows = [headers, ...rows];
  const widths = headers.map((_, c) => Math.max(...allRows.map((r) => String(r[c]).length)));
  return allRows
    .map((r, idx) => {
      const line = r.map((v, c) => String(v).padEnd(widths[c], ' ')).join(' | ');
      if (idx === 0) {
        const sep = widths.map((w) => '-'.repeat(w)).join('-|-');
        return `${line}\n${sep}`;
      }
      return line;
    })
    .join('\n');
}

function buildReport(data) {
  const weakRows = data.weakAnalysis.weak.map((row) => [
    row.key,
    row.cipherHex,
    row.avgChangedBits.toFixed(2),
    row.avgChangedHexChars.toFixed(2)
  ]);

  const semiRows = data.weakAnalysis.semiWeak.map((row) => [
    row.pair,
    row.cipherHex,
    row.avgChangedBits.toFixed(2),
    row.avgChangedHexChars.toFixed(2)
  ]);

  const avalanchePreview = data.avalanche.steps.map((step) => [
    step.bitPos,
    step.cipherHex,
    step.changedBits,
    step.changedHexChars
  ]);

  return [
    'Лабораторная работа: 7',
    'Вариант: 4 (DES-EEE2)',
    `Ключ: ${data.teacherParam}`,
    `Дата/время: ${new Date().toLocaleString()}`,
    '',
    '1) Реализация приложения',
    `- Исходный текст (фрагмент): ${data.sourceText.slice(0, 120)}`,
    `- K1: ${data.k1.toString('hex').toUpperCase()}`,
    `- K2: ${data.k2.toString('hex').toUpperCase()}`,
    `- Блоков после дополнения: ${data.blocks.length}`,
    `- Шифртекст (hex): ${data.cipher.toString('hex').toUpperCase()}`,
    `- Успешность расшифрования: ${data.decrypted.equals(Buffer.from(data.sourceText, 'utf8')) ? 'ДА' : 'НЕТ'}`,
    '',
    '2) Оценка скорости',
    `- Итераций: ${data.speed.iterations}`,
    `- Время шифрования: ${data.speed.encryptMs.toFixed(2)} мс`,
    `- Время расшифрования: ${data.speed.decryptMs.toFixed(2)} мс`,
    `- Скорость шифрования: ${Math.round(data.speed.encryptPerSec)} байт/с`,
    `- Скорость расшифрования: ${Math.round(data.speed.decryptPerSec)} байт/с`,
    '',
    '3) Лавинный эффект (пошагово, изменение одного бита во входном слове)',
    `- Базовый блок: ${data.avalanche.sourceBlockHex}`,
    `- Базовый шифртекст: ${data.avalanche.baseCipherHex}`,
    `- Среднее число изменившихся бит: ${data.avalanche.averageChangedBits.toFixed(2)} из 64`,
    `- Среднее число изменившихся hex-символов: ${data.avalanche.averageChangedHexChars.toFixed(2)} из 16`,
    '',
    formatTable(avalanchePreview, ['Бит', 'Шифртекст (hex)', 'Изм. бит', 'Изм. hex-симв.']),
    '',
    '4) Влияние слабых и полуслабых ключей (DES)',
    `- Базовые (обычные) ключи: ${data.weakAnalysis.baseline.keys}`,
    `- Базовый шифртекст: ${data.weakAnalysis.baseline.cipherHex}`,
    `- Базовый средний лавинный эффект: ${data.weakAnalysis.baseline.avgChangedBits.toFixed(2)} бит`,
    '',
    'Слабые ключи:',
    formatTable(weakRows, ['Ключ', 'Шифртекст (hex)', 'Ср. изм. бит', 'Ср. изм. hex']),
    '',
    'Полуслабые пары ключей:',
    formatTable(semiRows, ['Пара ключей K1/K2', 'Шифртекст (hex)', 'Ср. изм. бит', 'Ср. изм. hex']),
    '',
    '5) Оценка сжатия (gzip, level=9)',
    `- Исходный текст: ${data.compression.plainBytes} байт -> ${data.compression.gzPlainBytes} байт`,
    `- Коэффициент сжатия исходного: ${data.compression.plainRatioPercent.toFixed(2)}% от исходного объёма`,
    `- Степень сжатия исходного: ${data.compression.plainCompressionPercent.toFixed(2)}%`,
    `- Шифртекст: ${data.compression.cipherBytes} байт -> ${data.compression.gzCipherBytes} байт`,
    `- Коэффициент сжатия шифртекста: ${data.compression.cipherRatioPercent.toFixed(2)}% от исходного объёма`,
    `- Степень сжатия шифртекста: ${data.compression.cipherCompressionPercent.toFixed(2)}%`,
    '',
    'Пояснение:',
    'Шифртекст близок к случайной последовательности, поэтому содержит минимум статистической избыточности',
    'и обычно сжимается хуже открытого текста. Это соответствует теории для блочных шифров.',
    ''
  ].join('\n');
}

async function getInputText(rl) {
  const mode = await ask(rl, 'Источник текста: 1) вручную, 2) из файла', '1');
  if (mode === '2') {
    const filePath = await ask(rl, 'Путь к текстовому файлу');
    const abs = path.isAbsolute(filePath) ? filePath : path.join(process.cwd(), filePath);
    return fs.readFileSync(abs, 'utf8');
  }
  const inline = await ask(rl, 'Введите текст для шифрования', 'DES-EEE2');
  return inline;
}

async function pickKeys(rl, teacherMode) {
  if (teacherMode === '2') {
    const pick = await ask(rl, 'Номер слабого ключа (1-4)', '1');
    const idx = Math.max(1, Math.min(4, Number(pick) || 1)) - 1;
    const keyHex = WEAK_KEYS[idx];
    return { k1: Buffer.from(keyHex, 'hex'), k2: Buffer.from(keyHex, 'hex') };
  }

  if (teacherMode === '3') {
    const pick = await ask(rl, 'Номер полуслабой пары (1-6)', '1');
    const idx = Math.max(1, Math.min(6, Number(pick) || 1)) - 1;
    const [k1h, k2h] = SEMI_WEAK_PAIRS[idx];
    return { k1: Buffer.from(k1h, 'hex'), k2: Buffer.from(k2h, 'hex') };
  }

  const k1Input = await ask(rl, 'Введите K1 (16 hex)', crypto.randomBytes(8).toString('hex').toUpperCase());
  const k2Input = await ask(rl, 'Введите K2 (16 hex)', crypto.randomBytes(8).toString('hex').toUpperCase());
  return { k1: parseKeyHex(k1Input), k2: parseKeyHex(k2Input) };
}

async function main() {
  const rl = readline.createInterface({ input, output });

  try {
    runSelfTests();
    console.log('=== lab 7 ===');
    const teacherMode = await ask(
      rl,
      'Ключы (1 - обычные ключи, 2 - слабые ключи, 3 - полуслабые пары)',
      '1'
    );

    const sourceText = await getInputText(rl);
    const { k1, k2 } = await pickKeys(rl, teacherMode);

    const plain = Buffer.from(sourceText, 'utf8');
    const padded = padPkcs7(plain);
    const blocks = splitBlocks(padded);

    const cipher = desEee2Encrypt(plain, k1, k2);
    const decrypted = desEee2Decrypt(cipher, k1, k2);

    const iterationsInput = await ask(rl, 'Итераций для замера скорости', '200');
    const iterations = Math.max(1, Number(iterationsInput) || 200);

    const speed = measureSpeed(plain, k1, k2, iterations);
    const avalanche = analyzeAvalancheByPlainBit(sourceText, k1, k2);

    const strongK1 = crypto.randomBytes(8);
    const strongK2 = crypto.randomBytes(8);
    const weakAnalysis = analyzeWeakAndSemiWeak(sourceText, strongK1, strongK2);

    const compression = compressionStats(plain, cipher);

    const outputDir = ensureOutputDir();
    const reportPath = path.join(outputDir, 'report_lab7.txt');
    const cipherPath = path.join(outputDir, 'cipher.bin');
    const decryptPath = path.join(outputDir, 'decrypted.txt');
    const jsonPath = path.join(outputDir, 'results.json');

    const result = {
      teacherParam: teacherMode,
      sourceText,
      k1,
      k2,
      blocks,
      cipher,
      decrypted,
      speed,
      avalanche,
      weakAnalysis,
      compression
    };

    fs.writeFileSync(reportPath, buildReport(result), 'utf8');
    fs.writeFileSync(cipherPath, cipher);
    fs.writeFileSync(decryptPath, decrypted.toString('utf8'), 'utf8');
    fs.writeFileSync(
      jsonPath,
      JSON.stringify(
        {
          teacherParam: teacherMode,
          sourceText,
          k1: k1.toString('hex').toUpperCase(),
          k2: k2.toString('hex').toUpperCase(),
          blocksHex: blocks.map((b) => b.toString('hex').toUpperCase()),
          cipherHex: cipher.toString('hex').toUpperCase(),
          decryptedText: decrypted.toString('utf8'),
          speed,
          avalanche: {
            ...avalanche,
            steps: avalanche.steps
          },
          weakAnalysis,
          compression
        },
        null,
        2
      ),
      'utf8'
    );

    console.log('\n--- Краткий итог ---');
    console.log(`K1: ${k1.toString('hex').toUpperCase()}`);
    console.log(`K2: ${k2.toString('hex').toUpperCase()}`);
    console.log(`Блоков: ${blocks.length}`);
    console.log(`Шифртекст (hex): ${cipher.toString('hex').toUpperCase()}`);
    console.log(`Расшифрованный текст совпадает: ${decrypted.equals(plain) ? 'ДА' : 'НЕТ'}`);
    console.log(`Ср. лавинный эффект: ${avalanche.averageChangedBits.toFixed(2)} бит из 64`);
    console.log(`Сжатие текста: ${compression.plainCompressionPercent.toFixed(2)}%`);
    console.log(`Сжатие шифртекста: ${compression.cipherCompressionPercent.toFixed(2)}%`);
    console.log(`\nОтчет: ${reportPath}`);
    console.log(`JSON:  ${jsonPath}`);
    console.log(`Cipher:${cipherPath}`);
    console.log(`Plain: ${decryptPath}`);
  } catch (error) {
    console.error(`Ошибка: ${error.message}`);
    process.exitCode = 1;
  } finally {
    rl.close();
  }
}

main();
