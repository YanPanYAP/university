#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');
const readline = require('readline/promises');
const { stdin: input, stdout: output } = require('process');
const {
  DEFAULT_ITERATIONS,
  DEFAULT_MESSAGE,
  DEFAULT_SIZES,
  benchmarkHash,
  buildReport,
  computeHash,
  getAlgorithmInfo
} = require('./src/hashLab');

const OUTPUT_DIR = path.join(__dirname, 'output');

function ensureOutputDir() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
}

function parseArgs(argv) {
  const args = {
    algorithm: 'sha256',
    message: null,
    iterations: DEFAULT_ITERATIONS,
    noPrompt: false
  };
  for (let i = 0; i < argv.length; i += 1) {
    if (argv[i] === '--algorithm' && argv[i + 1]) args.algorithm = argv[++i];
    else if (argv[i] === '--message' && argv[i + 1]) args.message = argv[++i];
    else if (argv[i] === '--iterations' && argv[i + 1]) args.iterations = Number(argv[++i]);
    else if (argv[i] === '--no-prompt') args.noPrompt = true;
  }
  return args;
}

async function ask(rl, question, fallback) {
  const answer = await rl.question(`${question} [${fallback}]: `);
  return answer.trim() || fallback;
}

function saveOutputs({ algorithm, message, iterations }) {
  ensureOutputDir();
  const benchmarkRows = benchmarkHash({ algorithm, sizes: DEFAULT_SIZES, iterations });
  const report = buildReport({ algorithm, message, benchmarkRows });
  const digest = computeHash(message, algorithm);

  fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab9.txt'), report, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'hash_result.txt'), digest, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'benchmark_results.json'), JSON.stringify(benchmarkRows, null, 2), 'utf8');
  fs.writeFileSync(
    path.join(OUTPUT_DIR, 'benchmark_results.csv'),
    [
      'input_bytes,iterations,mean_ms,min_ms,max_ms,mb_per_sec,digest_hex_length',
      ...benchmarkRows.map((row) => [
        row.inputBytes,
        row.iterations,
        row.meanMs.toFixed(6),
        row.minMs.toFixed(6),
        row.maxMs.toFixed(6),
        row.mbPerSec.toFixed(2),
        row.digestHexLength
      ].join(','))
    ].join('\n'),
    'utf8'
  );

  return { digest, benchmarkRows, reportPath: path.join(OUTPUT_DIR, 'report_lab9.txt') };
}

async function main() {
  const args = parseArgs(process.argv.slice(2));
  const interactive = process.stdin.isTTY && !args.noPrompt;
  let algorithm = args.algorithm;
  let message = args.message || DEFAULT_MESSAGE;
  let iterations = args.iterations;

  if (interactive) {
    const rl = readline.createInterface({ input, output });
    try {
      algorithm = await ask(rl, 'Алгоритм хеширования (md5, sha1, sha256, sha512)', algorithm);
      message = await ask(rl, 'Входное сообщение', message);
      iterations = Number(await ask(rl, 'Итераций для замера скорости', String(iterations)));
    } finally {
      rl.close();
    }
  }

  const info = getAlgorithmInfo(algorithm);
  if (!Number.isInteger(iterations) || iterations <= 0) {
    throw new Error('iterations должен быть положительным целым.');
  }

  console.log(`ЛР №9: ${info.label}`);
  console.log(`Сообщение: ${message}`);
  const { digest, benchmarkRows, reportPath } = saveOutputs({ algorithm: info.id, message, iterations });
  console.log(`H(M) = ${digest}`);
  console.log(`Замеров быстродействия: ${benchmarkRows.length}`);
  console.log(`Отчет: ${reportPath}`);
}

main().catch((err) => {
  console.error('Ошибка:', err.message);
  process.exitCode = 1;
});
