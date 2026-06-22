#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const crypto = require('crypto');
const readline = require('readline/promises');
const { stdin: input, stdout: output } = require('process');

const OUTPUT_DIR = path.join(process.cwd(), 'output');
const BASE64_ALPHABET = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/';
const DEFAULT_FIO = 'Pavlovich Yan Andreevich';
const DEFAULT_ITERS = 300;
const DEFAULT_LAST_BITS = 100;

function ensureOutputDir() {
  if (!fs.existsSync(OUTPUT_DIR)) {
    fs.mkdirSync(OUTPUT_DIR, { recursive: true });
  }
}

function ask(rl, question, fallback = '') {
  return rl.question(`${question}${fallback ? ` [${fallback}]` : ''}: `).then((answer) => {
    const trimmed = answer.trim();
    return trimmed || fallback;
  });
}

function parseArgs(argv) {
  const args = { fio: null, iterations: null };
  for (let i = 0; i < argv.length; i += 1) {
    const token = argv[i];
    if (token === '--fio' && i + 1 < argv.length) {
      args.fio = argv[i + 1];
      i += 1;
    } else if (token === '--iterations' && i + 1 < argv.length) {
      args.iterations = argv[i + 1];
      i += 1;
    }
  }
  return args;
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

  return { gcd: oldR, x: oldS, y: oldT };
}

function mod(n, m) {
  const r = n % m;
  return r < 0n ? r + m : r;
}

function modInverse(a, n) {
  const { gcd: g, x } = egcd(a, n);
  if (g !== 1n) {
    throw new Error(`Обратный элемент не существует: gcd(${a}, ${n}) = ${g}`);
  }
  return mod(x, n);
}

function randomBigInt(bits) {
  if (!Number.isInteger(bits) || bits <= 0) {
    throw new Error('bits должен быть положительным целым числом');
  }
  const bytes = Math.ceil(bits / 8);
  const buf = crypto.randomBytes(bytes);
  const extraBits = (bytes * 8) - bits;
  if (extraBits > 0) {
    buf[0] &= (0xff >>> extraBits);
  }
  buf[0] |= (1 << (7 - extraBits));
  return BigInt(`0x${buf.toString('hex')}`);
}

function randomBigIntWithBits(bits) {
  if (!Number.isInteger(bits) || bits <= 0) {
    throw new Error('bits должен быть положительным целым числом');
  }
  const bytes = Math.ceil(bits / 8);
  const buf = crypto.randomBytes(bytes);
  const extraBits = (bytes * 8) - bits;
  if (extraBits > 0) {
    buf[0] &= (0xff >>> extraBits);
  }
  return BigInt(`0x${buf.toString('hex')}`);
}

function randomBigIntBelow(limitExclusive) {
  const limit = BigInt(limitExclusive);
  if (limit <= 1n) return 0n;
  const bits = limit.toString(2).length;
  while (true) {
    const candidate = randomBigIntWithBits(bits);
    if (candidate < limit) return candidate;
  }
}

function randomBigIntBetween(minInclusive, maxInclusive) {
  const min = BigInt(minInclusive);
  const max = BigInt(maxInclusive);
  if (max < min) {
    throw new Error('Неверный диапазон генерации BigInt');
  }
  const span = max - min + 1n;
  return min + randomBigIntBelow(span);
}

function generateSuperincreasingSequence(z, lastBits = DEFAULT_LAST_BITS) {
  if (!Number.isInteger(z) || z < 3) {
    throw new Error('z должно быть целым числом >= 3');
  }
  const seq = [];
  let sum = 0n;

  const first = randomBigIntBetween(2n, 500n);
  seq.push(first);
  sum += first;

  for (let i = 1; i < z - 1; i += 1) {
    const slack = randomBigIntBetween(1n, 65535n);
    const value = sum + slack;
    seq.push(value);
    sum += value;
  }

  const minLastByBits = 1n << BigInt(lastBits - 1);
  const maxLastByBits = (1n << BigInt(lastBits)) - 1n;
  const minLast = sum + 1n > minLastByBits ? sum + 1n : minLastByBits;
  if (minLast > maxLastByBits) {
    throw new Error('Не удалось построить последовательность: слишком большое z для 100-битного старшего члена');
  }
  const last = randomBigIntBetween(minLast, maxLastByBits);
  seq.push(last);
  return seq;
}

function generateModulusAndMultiplier(secretSeq) {
  const sum = secretSeq.reduce((acc, x) => acc + x, 0n);
  const n = sum + randomBigIntBetween(2n, 1_000_000n);
  let a = randomBigIntBetween(2n, n - 2n);
  while (gcd(a, n) !== 1n) {
    a = randomBigIntBetween(2n, n - 2n);
  }
  return { n, a };
}

function generateKeyPair(z, lastBits = DEFAULT_LAST_BITS) {
  const d = generateSuperincreasingSequence(z, lastBits);
  const { n, a } = generateModulusAndMultiplier(d);
  const e = d.map((di) => mod(di * a, n));
  const aInv = modInverse(a, n);
  return { z, d, e, n, a, aInv };
}

function numberToBits(value, width) {
  const bits = new Array(width).fill(0);
  for (let i = width - 1; i >= 0; i -= 1) {
    bits[i] = value & 1;
    value >>= 1;
  }
  return bits;
}

function bitsToNumber(bits) {
  let value = 0;
  for (const bit of bits) {
    value = (value << 1) | bit;
  }
  return value;
}

function unitsToBits(units, width) {
  const bits = [];
  for (const u of units) {
    bits.push(...numberToBits(u, width));
  }
  return bits;
}

function bitsToUnits(bits, width) {
  if (bits.length % width !== 0) {
    throw new Error('Некорректная длина битовой последовательности для декодирования');
  }
  const units = [];
  for (let i = 0; i < bits.length; i += width) {
    units.push(bitsToNumber(bits.slice(i, i + width)));
  }
  return units;
}

function encodeASCII(message) {
  const buf = Buffer.from(message, 'utf8');
  return {
    scheme: 'ASCII',
    bitWidth: 8,
    units: Array.from(buf.values()),
    encodedText: buf.toString('latin1')
  };
}

function decodeASCII(units) {
  return Buffer.from(units).toString('utf8');
}

function encodeBase64(message) {
  const base64 = Buffer.from(message, 'utf8').toString('base64');
  const clean = base64.replace(/=+$/g, '');
  const units = [];
  for (const ch of clean) {
    const idx = BASE64_ALPHABET.indexOf(ch);
    if (idx === -1) {
      throw new Error(`Неожиданный символ Base64: ${ch}`);
    }
    units.push(idx);
  }
  return {
    scheme: 'Base64',
    bitWidth: 6,
    units,
    encodedText: base64
  };
}

function decodeBase64(units, base64Length) {
  const chars = units.map((u) => BASE64_ALPHABET[u]).join('');
  const padLen = (4 - (chars.length % 4)) % 4;
  const withPad = chars + '='.repeat(padLen);
  const decoded = Buffer.from(withPad, 'base64').toString('utf8');

  if (base64Length != null) {
    const expected = chars.length + '='.repeat((4 - (chars.length % 4)) % 4);
    if (expected.length !== base64Length) {
      return decoded;
    }
  }
  return decoded;
}

function packBitsToBlocks(bits, z) {
  const padBits = (z - (bits.length % z)) % z;
  const padded = bits.slice();
  for (let i = 0; i < padBits; i += 1) padded.push(0);
  const blocks = [];
  for (let i = 0; i < padded.length; i += z) {
    blocks.push(padded.slice(i, i + z));
  }
  return { blocks, padBits };
}

function solveSuperincreasing(sum, seq) {
  let rest = BigInt(sum);
  const bits = new Array(seq.length).fill(0);
  for (let i = seq.length - 1; i >= 0; i -= 1) {
    if (seq[i] <= rest) {
      bits[i] = 1;
      rest -= seq[i];
    }
  }
  if (rest !== 0n) {
    throw new Error('Расшифрование невозможно: сумма не представима в супервозрастающей последовательности');
  }
  return bits;
}

function encryptBits(blocks, publicKey) {
  return blocks.map((blockBits) => {
    let sum = 0n;
    for (let i = 0; i < blockBits.length; i += 1) {
      if (blockBits[i] === 1) sum += publicKey[i];
    }
    return sum;
  });
}

function decryptToBits(cipherBlocks, keys) {
  const outBits = [];
  for (const c of cipherBlocks) {
    const s = mod(c * keys.aInv, keys.n);
    const bits = solveSuperincreasing(s, keys.d);
    outBits.push(...bits);
  }
  return outBits;
}

function encryptMessage(message, scheme, keys) {
  const encoded = scheme === 'ASCII' ? encodeASCII(message) : encodeBase64(message);
  const sourceBits = unitsToBits(encoded.units, encoded.bitWidth);
  const { blocks, padBits } = packBitsToBlocks(sourceBits, keys.z);
  const cipher = encryptBits(blocks, keys.e);
  return {
    scheme: encoded.scheme,
    encodedText: encoded.encodedText,
    units: encoded.units,
    bitWidth: encoded.bitWidth,
    blocks,
    cipher,
    originalBitsLength: sourceBits.length,
    padBits,
    base64Length: scheme === 'Base64' ? encoded.encodedText.length : null
  };
}

function decryptMessage(payload, keys) {
  const allBits = decryptToBits(payload.cipher, keys).slice(0, payload.originalBitsLength);
  const units = bitsToUnits(allBits, payload.bitWidth);
  const message = payload.scheme === 'ASCII'
    ? decodeASCII(units)
    : decodeBase64(units, payload.base64Length);
  return message;
}

function toMs(start, end) {
  return Number(end - start) / 1e6;
}

function benchmarkOne(message, scheme, z, iterations) {
  const keys = generateKeyPair(z, DEFAULT_LAST_BITS);
  let encTotal = 0n;
  let decTotal = 0n;
  let valid = true;
  let sampleCipher = [];

  for (let i = 0; i < iterations; i += 1) {
    const t1 = process.hrtime.bigint();
    const encrypted = encryptMessage(message, scheme, keys);
    const t2 = process.hrtime.bigint();
    const decrypted = decryptMessage(encrypted, keys);
    const t3 = process.hrtime.bigint();

    if (i === 0) sampleCipher = encrypted.cipher.slice(0, 3).map(String);
    if (decrypted !== message) valid = false;
    encTotal += (t2 - t1);
    decTotal += (t3 - t2);
  }

  const avgEncMs = toMs(0n, encTotal) / iterations;
  const avgDecMs = toMs(0n, decTotal) / iterations;
  return {
    scheme,
    z,
    iterations,
    avgEncryptMs: avgEncMs,
    avgDecryptMs: avgDecMs,
    speedEncryptOpsPerSec: 1000 / avgEncMs,
    speedDecryptOpsPerSec: 1000 / avgDecMs,
    valid,
    sampleCipher
  };
}

function theoreticalSecurity(z) {
  const brute = 1n << BigInt(z);
  const greedyOps = z;
  return {
    z,
    bruteForceVariants: brute.toString(),
    greedyDecryptComplexity: `O(${greedyOps})`,
    note: 'Классический Merkle-Hellman криптоаналитически уязвим (атака Шамира), поэтому высокая практическая стойкость не достигается.'
  };
}

function formatBigIntList(list, max = 12) {
  if (list.length <= max) return list.map(String).join(', ');
  const head = list.slice(0, Math.floor(max / 2)).map(String).join(', ');
  const tail = list.slice(-(Math.floor(max / 2))).map(String).join(', ');
  return `${head}, ..., ${tail}`;
}

function formatBitBlock(bits) {
  return bits.join('');
}

function formatKnapsackTerms(bits, key) {
  const terms = [];
  for (let i = 0; i < bits.length; i += 1) {
    if (bits[i] === 1) terms.push(key[i].toString());
  }
  return terms.length > 0 ? terms.join(' + ') : '0';
}

function formatVisibleChar(ch) {
  if (ch === ' ') return '<space>';
  if (ch === '\n') return '<LF>';
  if (ch === '\r') return '<CR>';
  if (ch === '\t') return '<TAB>';
  return ch;
}

function appendEncodingTable(lines, main) {
  const meta = main.encryptedMeta;
  if (meta.scheme === 'ASCII') {
    lines.push('Таблица кодирования ASCII:');
    lines.push('Символ | Код ASCII | Бинарный код');
    for (const code of meta.units) {
      lines.push([
        formatVisibleChar(String.fromCharCode(code)),
        code.toString(),
        formatBitBlock(numberToBits(code, meta.bitWidth))
      ].join(' | '));
    }
    return;
  }

  lines.push(`Base64-представление сообщения: ${meta.encodedText}`);
  lines.push('Таблица кодирования Base64:');
  lines.push('Символ Base64 | Индекс в алфавите | Бинарный код');
  const cleanBase64 = meta.encodedText.replace(/=+$/g, '');
  for (let i = 0; i < cleanBase64.length; i += 1) {
    const ch = cleanBase64[i];
    const index = meta.units[i];
    lines.push([
      ch,
      index.toString(),
      formatBitBlock(numberToBits(index, meta.bitWidth))
    ].join(' | '));
  }
}

function appendEncryptionTable(lines, main) {
  lines.push('Таблица шифрования:');
  lines.push('Бинарный код блока | Укладка ранца (e_i) | Вес ранца C');
  for (let i = 0; i < main.encryptedMeta.blocks.length; i += 1) {
    const bits = main.encryptedMeta.blocks[i];
    lines.push([
      formatBitBlock(bits),
      formatKnapsackTerms(bits, main.keys.e),
      main.cipher[i].toString()
    ].join(' | '));
  }
}

function appendDecryptionTable(lines, main) {
  lines.push('Таблица дешифрования:');
  lines.push('C | S = C * a^-1 mod n | Укладка ранца (d_i) | Восстановленный блок');
  for (let i = 0; i < main.cipher.length; i += 1) {
    const c = main.cipher[i];
    const s = mod(c * main.keys.aInv, main.keys.n);
    const bits = solveSuperincreasing(s, main.keys.d);
    lines.push([
      c.toString(),
      s.toString(),
      formatKnapsackTerms(bits, main.keys.d),
      formatBitBlock(bits)
    ].join(' | '));
  }
}

function makeReport(params) {
  const {
    fio,
    iterations,
    asciiMain,
    base64Main,
    asciiBench,
    base64Bench
  } = params;

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`ФИО: ${fio}`);
  lines.push(`Итераций для оценки времени: ${iterations}`);
  lines.push(`Дата: ${new Date().toISOString()}`);
  lines.push('');
  lines.push('');
  lines.push('2. Генерация ключей и шифрование/расшифрование');
  lines.push('');
  lines.push('2.1 ASCII (z = 8)');
  lines.push('Закрытый ключ d (супервозрастающий):');
  lines.push(formatBigIntList(asciiMain.keys.d));
  lines.push('');
  lines.push('Параметры:');
  lines.push(`n=${asciiMain.keys.n},`);
  lines.push(`a=${asciiMain.keys.a},`);
  lines.push(`a^-1 mod n=${asciiMain.keys.aInv}`);
  lines.push('');
  lines.push('Открытый ключ e:');
  lines.push(formatBigIntList(asciiMain.keys.e));
  lines.push('');
  lines.push('Шифртекст C:');
  lines.push(asciiMain.cipher.map(String).join(' '));
  lines.push('');
  appendEncodingTable(lines, asciiMain);
  lines.push('');
  appendEncryptionTable(lines, asciiMain);
  lines.push('');
  appendDecryptionTable(lines, asciiMain);
  lines.push('');
  lines.push('Дешифрованный текст:');
  lines.push(asciiMain.decrypted);
  lines.push('');
  lines.push(`Проверка корректности: ${asciiMain.decrypted === fio ? 'OK' : 'FAILED'}`);
  lines.push('');
  lines.push('');
  lines.push('2.2 Base64 (z = 6)');
  lines.push('Закрытый ключ d (супервозрастающий):');
  lines.push(formatBigIntList(base64Main.keys.d));
  lines.push('');
  lines.push('Параметры:');
  lines.push(`n=${base64Main.keys.n},`);
  lines.push(`a=${base64Main.keys.a},`);
  lines.push(`a^-1 mod n=${base64Main.keys.aInv}`);
  lines.push('');
  lines.push('Открытый ключ e:');
  lines.push(formatBigIntList(base64Main.keys.e));
  lines.push('');
  lines.push('Шифртекст C:');
  lines.push(base64Main.cipher.map(String).join(' '));
  lines.push('');
  appendEncodingTable(lines, base64Main);
  lines.push('');
  appendEncryptionTable(lines, base64Main);
  lines.push('');
  appendDecryptionTable(lines, base64Main);
  lines.push('');
  lines.push('Дешифрованный текст:');
  lines.push(base64Main.decrypted);
  lines.push('');
  lines.push(`Проверка корректности: ${base64Main.decrypted === fio ? 'OK' : 'FAILED'}`);
  lines.push('');
  lines.push('');
  lines.push('3. Оценка криптостойкости');
  lines.push('');

  const allZ = [...asciiBench, ...base64Bench]
    .map((r) => ({ z: r.z, scheme: r.scheme, brute: theoreticalSecurity(r.z).bruteForceVariants }))
    .sort((a, b) => a.z - b.z);
  for (const row of allZ) {
    lines.push(`${row.scheme} z=${row.z}: 2^z = ${row.brute}`);
  }

  lines.push('');
  lines.push('');
  lines.push('4. Оценка времени шифрования/расшифрования');
  lines.push('');
  lines.push('ASCII:');
  for (const r of asciiBench) {
    lines.push(
      `z=${r.z}: enc=${r.avgEncryptMs.toFixed(4)} ms, dec=${r.avgDecryptMs.toFixed(4)} ms, ` +
      `enc=${r.speedEncryptOpsPerSec.toFixed(2)} оп/с, dec=${r.speedDecryptOpsPerSec.toFixed(2)} оп/с`
    );
  }
  lines.push('');
  lines.push('Base64:');
  for (const r of base64Bench) {
    lines.push(
      `z=${r.z}: enc=${r.avgEncryptMs.toFixed(4)} ms, dec=${r.avgDecryptMs.toFixed(4)} ms, ` +
      `enc=${r.speedEncryptOpsPerSec.toFixed(2)} оп/с, dec=${r.speedDecryptOpsPerSec.toFixed(2)} оп/с`
    );
  }

  return lines.join('\n');
}

function runMainExperiment(fio) {
  const asciiKeys = generateKeyPair(8, DEFAULT_LAST_BITS);
  const base64Keys = generateKeyPair(6, DEFAULT_LAST_BITS);

  const asciiEncrypted = encryptMessage(fio, 'ASCII', asciiKeys);
  const base64Encrypted = encryptMessage(fio, 'Base64', base64Keys);

  const asciiDecrypted = decryptMessage(asciiEncrypted, asciiKeys);
  const base64Decrypted = decryptMessage(base64Encrypted, base64Keys);

  return {
    ascii: {
      keys: asciiKeys,
      cipher: asciiEncrypted.cipher,
      encryptedMeta: asciiEncrypted,
      decrypted: asciiDecrypted
    },
    base64: {
      keys: base64Keys,
      cipher: base64Encrypted.cipher,
      encryptedMeta: base64Encrypted,
      decrypted: base64Decrypted
    }
  };
}

async function main() {
  ensureOutputDir();
  const cliArgs = parseArgs(process.argv.slice(2));
  const interactive = process.stdin.isTTY;
  const rl = interactive ? readline.createInterface({ input, output }) : null;

  try {
    console.log('Лабораторная: исследование асимметричных шифров (Merkle-Hellman).');
    const fio = interactive
      ? await ask(rl, 'Введите ФИО (латиницей или UTF-8)', cliArgs.fio || DEFAULT_FIO)
      : (cliArgs.fio || DEFAULT_FIO);
    const iterationsRaw = interactive
      ? await ask(rl, 'Итераций для замеров времени', cliArgs.iterations || String(DEFAULT_ITERS))
      : (cliArgs.iterations || String(DEFAULT_ITERS));
    const iterations = Number(iterationsRaw);
    if (!Number.isInteger(iterations) || iterations <= 0) {
      throw new Error('Количество итераций должно быть положительным целым числом');
    }

    const experiment = runMainExperiment(fio);
    const asciiMain = experiment.ascii;
    const base64Main = experiment.base64;

    if (asciiMain.decrypted !== fio || base64Main.decrypted !== fio) {
      throw new Error('Проверка корректности расшифрования не пройдена');
    }

    const asciiZ = [8, 16, 24, 32];
    const base64Z = [6, 12, 18, 24];
    const asciiBench = asciiZ.map((z) => benchmarkOne(fio, 'ASCII', z, iterations));
    const base64Bench = base64Z.map((z) => benchmarkOne(fio, 'Base64', z, iterations));

    const report = makeReport({
      fio,
      iterations,
      asciiMain,
      base64Main,
      asciiBench,
      base64Bench
    });

    const resultsJson = {
      fio,
      generatedAt: new Date().toISOString(),
      mainExperiment: {
        ascii: {
          z: asciiMain.keys.z,
          n: asciiMain.keys.n.toString(),
          a: asciiMain.keys.a.toString(),
          aInv: asciiMain.keys.aInv.toString(),
          d: asciiMain.keys.d.map(String),
          e: asciiMain.keys.e.map(String),
          cipher: asciiMain.cipher.map(String),
          decrypted: asciiMain.decrypted
        },
        base64: {
          z: base64Main.keys.z,
          n: base64Main.keys.n.toString(),
          a: base64Main.keys.a.toString(),
          aInv: base64Main.keys.aInv.toString(),
          d: base64Main.keys.d.map(String),
          e: base64Main.keys.e.map(String),
          cipher: base64Main.cipher.map(String),
          decrypted: base64Main.decrypted
        }
      },
      benchmark: {
        iterations,
        ascii: asciiBench,
        base64: base64Bench
      },
      securityNotes: [
        'Перебор подмножеств для общего ранца имеет сложность порядка 2^z.',
        'Для супервозрастающей последовательности расшифрование выполняется жадным алгоритмом O(z).',
        'Классический Merkle-Hellman уязвим к криптоанализу (Шамир), поэтому не используется в современных защищенных системах.'
      ]
    };

    fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab9.txt'), report, 'utf8');
    fs.writeFileSync(path.join(OUTPUT_DIR, 'results.json'), JSON.stringify(resultsJson, null, 2), 'utf8');
    fs.writeFileSync(
      path.join(OUTPUT_DIR, 'cipher_ascii.txt'),
      asciiMain.cipher.map(String).join(' '),
      'utf8'
    );
    fs.writeFileSync(
      path.join(OUTPUT_DIR, 'cipher_base64.txt'),
      base64Main.cipher.map(String).join(' '),
      'utf8'
    );
    fs.writeFileSync(path.join(OUTPUT_DIR, 'decrypted_ascii.txt'), asciiMain.decrypted, 'utf8');
    fs.writeFileSync(path.join(OUTPUT_DIR, 'decrypted_base64.txt'), base64Main.decrypted, 'utf8');

    console.log('Готово. Сформированы файлы:');
    console.log(`- ${path.join(OUTPUT_DIR, 'report_lab9.txt')}`);
    console.log(`- ${path.join(OUTPUT_DIR, 'results.json')}`);
    console.log(`- ${path.join(OUTPUT_DIR, 'cipher_ascii.txt')}`);
    console.log(`- ${path.join(OUTPUT_DIR, 'cipher_base64.txt')}`);
    console.log(`- ${path.join(OUTPUT_DIR, 'decrypted_ascii.txt')}`);
    console.log(`- ${path.join(OUTPUT_DIR, 'decrypted_base64.txt')}`);
  } finally {
    if (rl) rl.close();
  }
}

main().catch((err) => {
  console.error('Ошибка:', err.message);
  process.exitCode = 1;
});
