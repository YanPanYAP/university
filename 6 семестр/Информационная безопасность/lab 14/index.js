#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');

const OUTPUT_DIR = path.join(__dirname, 'output');
const VARIANT = 9;
const SECRET_MESSAGE = 'PAVLOVICH';
const LENGTH_BITS = 16;

const CONTAINER_TEXT = [
  'Text steganography hides a message inside an ordinary document without changing its visible meaning.',
  'In this laboratory work the container is a neutral paragraph about information security, digital documents, and reliable communication channels.',
  'The method based on spaces changes the number of blanks between neighboring words, while the method based on kerning changes the distance between adjacent letters.',
  'A reader sees the same sentence structure, but a program can measure the selected spatial parameters and restore the embedded sequence of bits.',
  'Such techniques are fragile after automatic formatting, therefore they are usually demonstrated on controlled documents where spacing and typography are preserved.',
  'The goal of the experiment is to encode a short secret message, save the modified containers, extract the message back, and compare the result with the original data.'
].join(' ');

function ensureOutputDir() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
}

function textToBits(text) {
  const bytes = Array.from(Buffer.from(text, 'utf8'));
  const lengthBits = bytes.length.toString(2).padStart(LENGTH_BITS, '0');
  const dataBits = bytes.map((byte) => byte.toString(2).padStart(8, '0')).join('');
  return lengthBits + dataBits;
}

function bitsToText(bits) {
  const byteLength = parseInt(bits.slice(0, LENGTH_BITS), 2);
  const payloadBits = bits.slice(LENGTH_BITS, LENGTH_BITS + byteLength * 8);
  const bytes = [];

  for (let i = 0; i < payloadBits.length; i += 8) {
    bytes.push(parseInt(payloadBits.slice(i, i + 8), 2));
  }

  return Buffer.from(bytes).toString('utf8');
}

function countWordGaps(text) {
  const matches = text.match(/\S+\s+\S+/g);
  if (!matches) return 0;
  return text.trim().split(/\s+/).length - 1;
}

function encodeBySpaces(container, bits) {
  const words = container.trim().split(/\s+/);
  if (words.length - 1 < bits.length) {
    throw new Error('Недостаточно промежутков между словами для встраивания сообщения.');
  }

  let out = words[0];
  for (let i = 1; i < words.length; i += 1) {
    const bit = bits[i - 1];
    const gap = bit === '1' ? '  ' : ' ';
    out += gap + words[i];
  }

  return out;
}

function extractFromSpaces(stegoText) {
  const bits = [];
  const regex = /\S+( +)(?=\S)/g;
  let match;

  while ((match = regex.exec(stegoText)) !== null) {
    bits.push(match[1].length > 1 ? '1' : '0');
  }

  return bitsToText(bits.join(''));
}

function escapeHtml(text) {
  return text
    .replace(/&/g, '&amp;')
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/"/g, '&quot;');
}

function encodeByKerning(container, bits) {
  if (container.length < bits.length) {
    throw new Error('Недостаточно символов контейнера для встраивания сообщения кернингом.');
  }

  let body = '';
  for (let i = 0; i < container.length; i += 1) {
    const ch = escapeHtml(container[i]);
    if (i < bits.length) {
      body += `<span class="k${bits[i]}">${ch}</span>`;
    } else {
      body += ch;
    }
  }

  return [
    '<!doctype html>',
    '<html lang="ru">',
    '<head>',
    '  <meta charset="utf-8">',
    '  <title>Lab 13 Kerning Steganography</title>',
    '  <style>',
    '    body { font-family: Georgia, serif; line-height: 1.65; max-width: 920px; margin: 40px auto; color: #202020; }',
    '    .k0 { letter-spacing: 0; }',
    '    .k1 { letter-spacing: 0.18em; }',
    '  </style>',
    '</head>',
    '<body>',
    `  <p>${body}</p>`,
    '</body>',
    '</html>'
  ].join('\n');
}

function extractFromKerning(html) {
  const bits = [];
  const regex = /<span class="k([01])">/g;
  let match;

  while ((match = regex.exec(html)) !== null) {
    bits.push(match[1]);
  }

  return bitsToText(bits.join(''));
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

function buildBitRows(bits, limit = 16) {
  return bits.slice(0, limit).split('').map((bit, index) => [
    index + 1,
    bit,
    bit === '0' ? '1 пробел' : '2 пробела',
    bit === '0' ? 'letter-spacing: 0' : 'letter-spacing: 0.18em'
  ]);
}

function buildReport({ bits, spaceResult, kerningResult }) {
  const rows = [
    [
      'Модификация числа пробелов',
      spaceResult.capacity,
      bits.length,
      spaceResult.extracted,
      spaceResult.ok ? 'OK' : 'FAIL'
    ],
    [
      'Модификация кернинга',
      kerningResult.capacity,
      bits.length,
      kerningResult.extracted,
      kerningResult.ok ? 'OK' : 'FAIL'
    ]
  ];
  const messageBytes = Array.from(Buffer.from(SECRET_MESSAGE, 'utf8'));
  const lengthBits = bits.slice(0, LENGTH_BITS);
  const payloadBits = bits.slice(LENGTH_BITS);
  const bitRows = buildBitRows(bits);

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`Лабораторная работа: №13`);
  lines.push(`Вариант: ${VARIANT}`);
  lines.push('Методы: модификация числа пробелов; модификация кернинга');
  lines.push(`Секретное сообщение: ${SECRET_MESSAGE}`);
  lines.push(`Длина сообщения: ${Buffer.byteLength(SECRET_MESSAGE, 'utf8')} байт`);
  lines.push(`Служебный заголовок длины: ${LENGTH_BITS} бит`);
  lines.push(`Всего встраиваемых бит: ${bits.length}`);
  lines.push('');
  lines.push('2. Контейнер');
  lines.push(`Длина контейнера: ${CONTAINER_TEXT.length} символов`);
  lines.push(`Количество промежутков между словами: ${countWordGaps(CONTAINER_TEXT)}`);
  lines.push('');
  lines.push('3. Схема кодирования');
  lines.push('Пробелы: 0 кодируется одним пробелом, 1 кодируется двумя пробелами между словами.');
  lines.push('Кернинг: 0 кодируется обычным расстоянием между символами, 1 кодируется увеличенным letter-spacing.');
  lines.push('');
  lines.push('4. Ход вычислений');
  lines.push(`M = ${SECRET_MESSAGE}`);
  lines.push(`bytes(M) = ${messageBytes.join(', ')}`);
  lines.push(`len(M) = ${messageBytes.length} байт = ${lengthBits}`);
  lines.push(`bits(M) = ${payloadBits}`);
  lines.push(`B = len16 || bits(M) = ${bits}`);
  lines.push(`|B| = ${LENGTH_BITS} + 8 * ${messageBytes.length} = ${bits.length} бит`);
  lines.push(`Емкость метода пробелов: C_space = число промежутков между словами = ${spaceResult.capacity} бит`);
  lines.push(`Емкость метода кернинга: C_kern = число символов контейнера = ${kerningResult.capacity} бит`);
  lines.push(`Условие встраивания: |B| <= C, то есть ${bits.length} <= ${spaceResult.capacity} и ${bits.length} <= ${kerningResult.capacity}`);
  lines.push('');
  lines.push('Первые шаги встраивания:');
  lines.push(formatTable(
    ['#', 'Бит', 'Пробелы', 'Кернинг'],
    bitRows,
    ['right', 'left', 'left', 'left']
  ));
  lines.push('');
  lines.push('5. Результаты');
  lines.push(formatTable(
    ['Метод', 'Емкость, бит', 'Встроено, бит', 'Извлечено', 'Проверка'],
    rows,
    ['left', 'right', 'right', 'left', 'left']
  ));

  return lines.join('\n');
}

function main() {
  ensureOutputDir();

  const bits = textToBits(SECRET_MESSAGE);
  const stegoSpaces = encodeBySpaces(CONTAINER_TEXT, bits);
  const extractedSpaces = extractFromSpaces(stegoSpaces);
  const stegoKerning = encodeByKerning(CONTAINER_TEXT, bits);
  const extractedKerning = extractFromKerning(stegoKerning);

  const spaceResult = {
    capacity: countWordGaps(CONTAINER_TEXT),
    extracted: extractedSpaces,
    ok: extractedSpaces === SECRET_MESSAGE
  };
  const kerningResult = {
    capacity: CONTAINER_TEXT.length,
    extracted: extractedKerning,
    ok: extractedKerning === SECRET_MESSAGE
  };
  const report = buildReport({ bits, spaceResult, kerningResult });

  fs.writeFileSync(path.join(OUTPUT_DIR, 'container.txt'), CONTAINER_TEXT, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'stego_spaces.txt'), stegoSpaces, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'stego_kerning.html'), stegoKerning, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab13.txt'), report, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'results.json'), JSON.stringify({
    variant: VARIANT,
    methods: ['space-count modification', 'kerning modification'],
    secretMessage: SECRET_MESSAGE,
    bits,
    embeddedBits: bits.length,
    container: {
      chars: CONTAINER_TEXT.length,
      wordGaps: countWordGaps(CONTAINER_TEXT)
    },
    results: {
      spaces: spaceResult,
      kerning: kerningResult
    }
  }, null, 2), 'utf8');

  console.log('ЛР №13: текстовая стеганография');
  console.log(`Вариант ${VARIANT}: пробелы и кернинг`);
  console.log(`Сообщение: ${SECRET_MESSAGE}`);
  console.log(`Встроено бит: ${bits.length}`);
  console.log(`Пробелы: извлечено "${extractedSpaces}", ${spaceResult.ok ? 'OK' : 'FAIL'}`);
  console.log(`Кернинг: извлечено "${extractedKerning}", ${kerningResult.ok ? 'OK' : 'FAIL'}`);
  console.log(`Отчет: ${path.join(OUTPUT_DIR, 'report_lab13.txt')}`);
}

main();
