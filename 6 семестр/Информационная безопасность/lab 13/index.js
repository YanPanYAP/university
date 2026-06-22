#!/usr/bin/env node
'use strict';

const fs = require('fs');
const path = require('path');

const OUTPUT_DIR = path.join(__dirname, 'output');

const VARIANT = 9;
const P_FIELD = 751;
const A = -1;
const B = 1;
const X_MIN = 516;
const X_MAX = 550;
const K = 8;
const L = 5;

const INF = null;

function ensureOutputDir() {
  fs.mkdirSync(OUTPUT_DIR, { recursive: true });
}

function mod(n, m = P_FIELD) {
  const r = n % m;
  return r < 0 ? r + m : r;
}

function egcd(a, b) {
  let oldR = mod(a, b);
  let r = b;
  let oldS = 1;
  let s = 0;

  while (r !== 0) {
    const q = Math.floor(oldR / r);
    [oldR, r] = [r, oldR - q * r];
    [oldS, s] = [s, oldS - q * s];
  }

  return { g: oldR, x: oldS };
}

function modInv(a, m = P_FIELD) {
  const { g, x } = egcd(a, m);
  if (g !== 1) throw new Error(`Нет обратного элемента для ${a} по модулю ${m}.`);
  return mod(x, m);
}

function pointToString(point) {
  return point === INF ? 'O' : `(${point.x}; ${point.y})`;
}

function isOnCurve(point) {
  if (point === INF) return true;
  const left = mod(point.y * point.y);
  const right = mod(point.x ** 3 + A * point.x + B);
  return left === right;
}

function negatePoint(point) {
  if (point === INF) return INF;
  return { x: point.x, y: mod(-point.y) };
}

function addPoints(p1, p2) {
  if (p1 === INF) return p2;
  if (p2 === INF) return p1;

  if (p1.x === p2.x && mod(p1.y + p2.y) === 0) {
    return INF;
  }

  let lambda;
  if (p1.x === p2.x && p1.y === p2.y) {
    lambda = mod((3 * p1.x * p1.x + A) * modInv(2 * p1.y));
  } else {
    lambda = mod((p2.y - p1.y) * modInv(p2.x - p1.x));
  }

  const x3 = mod(lambda * lambda - p1.x - p2.x);
  const y3 = mod(lambda * (p1.x - x3) - p1.y);
  return { x: x3, y: y3 };
}

function subtractPoints(p1, p2) {
  return addPoints(p1, negatePoint(p2));
}

function multiplyPoint(k, point) {
  let n = k;
  let result = INF;
  let addend = point;

  while (n > 0) {
    if (n & 1) result = addPoints(result, addend);
    addend = addPoints(addend, addend);
    n >>= 1;
  }

  return result;
}

function findCurvePoints() {
  const rows = [];
  const points = [];

  for (let x = X_MIN; x <= X_MAX; x += 1) {
    const rhs = mod(x ** 3 + A * x + B);
    const ys = [];
    for (let y = 0; y < P_FIELD; y += 1) {
      if (mod(y * y) === rhs) ys.push(y);
    }

    if (ys.length > 0) {
      rows.push({ x, rhs, ys });
      for (const y of ys) points.push({ x, y });
    }
  }

  return { rows, points };
}

function chooseBasePoints(points) {
  const selected = [];
  const usedX = new Set();

  for (const point of points) {
    if (usedX.has(point.x)) continue;
    selected.push(point);
    usedX.add(point.x);
    if (selected.length === 3) break;
  }

  if (selected.length < 3) {
    throw new Error('Недостаточно точек с различными x для выполнения операций.');
  }

  return { P: selected[0], Q: selected[1], R: selected[2] };
}

function runOperations(base) {
  const { P, Q, R } = base;
  const kP = multiplyPoint(K, P);
  const lQ = multiplyPoint(L, Q);

  return [
    {
      operation: 'kP',
      expression: `${K}${pointToString(P)}`,
      result: kP
    },
    {
      operation: 'P + Q',
      expression: `${pointToString(P)} + ${pointToString(Q)}`,
      result: addPoints(P, Q)
    },
    {
      operation: 'kP + lQ - R',
      expression: `${K}P + ${L}Q - R`,
      result: subtractPoints(addPoints(kP, lQ), R)
    },
    {
      operation: 'P - Q + R',
      expression: 'P - Q + R',
      result: addPoints(subtractPoints(P, Q), R)
    }
  ];
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

function buildReport({ foundRows, basePoints, operations }) {
  const pointRows = foundRows.map((row) => [
    row.x,
    row.rhs,
    row.ys.map((y) => `(${row.x}; ${y})`).join(', ')
  ]);
  const operationRows = operations.map((row) => [
    row.operation,
    row.expression,
    pointToString(row.result),
    isOnCurve(row.result) ? 'OK' : 'FAIL'
  ]);

  const lines = [];
  lines.push('1. Исходные данные');
  lines.push(`Вариант: ${VARIANT}`);
  lines.push(`Эллиптическая кривая: y^2 = x^3 - x + 1 (mod ${P_FIELD})`);
  lines.push(`a = ${A}, b = ${B}, p = ${P_FIELD}`);
  lines.push(`Диапазон x: ${X_MIN}..${X_MAX}`);
  lines.push(`k = ${K}, l = ${L}`);
  lines.push('');
  lines.push('2. Найденные точки ЭК');
  lines.push(formatTable(
    ['x', 'x^3 - x + 1 mod p', 'Точки'],
    pointRows,
    ['right', 'right', 'left']
  ));
  lines.push('');
  lines.push(`Всего найдено точек: ${foundRows.reduce((sum, row) => sum + row.ys.length, 0)}`);
  lines.push('');
  lines.push('3. Точки для операций');
  lines.push('Для воспроизводимости выбраны первые три найденные точки с различными x.');
  lines.push(`P = ${pointToString(basePoints.P)}`);
  lines.push(`Q = ${pointToString(basePoints.Q)}`);
  lines.push(`R = ${pointToString(basePoints.R)}`);
  lines.push('');
  lines.push('4. Операции над точками');
  lines.push(formatTable(
    ['Операция', 'Выражение', 'Результат', 'Проверка'],
    operationRows,
    ['left', 'left', 'left', 'left']
  ));

  return lines.join('\n');
}

function saveResults({ foundRows, basePoints, operations, report }) {
  ensureOutputDir();
  fs.writeFileSync(path.join(OUTPUT_DIR, 'report_lab11.txt'), report, 'utf8');
  fs.writeFileSync(path.join(OUTPUT_DIR, 'results.json'), JSON.stringify({
    variant: VARIANT,
    curve: { a: A, b: B, p: P_FIELD },
    range: { xMin: X_MIN, xMax: X_MAX },
    coefficients: { k: K, l: L },
    points: foundRows.flatMap((row) => row.ys.map((y) => ({ x: row.x, y }))),
    basePoints,
    operations: operations.map((row) => ({
      operation: row.operation,
      expression: row.expression,
      result: row.result,
      valid: isOnCurve(row.result)
    }))
  }, null, 2), 'utf8');
}

function main() {
  const { rows: foundRows, points } = findCurvePoints();
  const basePoints = chooseBasePoints(points);
  const operations = runOperations(basePoints);
  const report = buildReport({ foundRows, basePoints, operations });
  saveResults({ foundRows, basePoints, operations, report });

  console.log('ЛР №11: эллиптические кривые');
  console.log(`Вариант ${VARIANT}: x = ${X_MIN}..${X_MAX}, k = ${K}, l = ${L}`);
  console.log(`Найдено точек: ${points.length}`);
  console.log(`P=${pointToString(basePoints.P)}, Q=${pointToString(basePoints.Q)}, R=${pointToString(basePoints.R)}`);
  for (const row of operations) {
    console.log(`${row.operation}: ${pointToString(row.result)}`);
  }
  console.log(`Отчет: ${path.join(OUTPUT_DIR, 'report_lab11.txt')}`);
  console.log(`JSON: ${path.join(OUTPUT_DIR, 'results.json')}`);
}

main();
