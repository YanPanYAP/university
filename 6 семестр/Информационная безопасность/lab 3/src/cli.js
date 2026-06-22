const path = require("path");
const {
  CONCATENATED_NUMBER,
  FIRST_NUMBER,
  SECOND_NUMBER,
  VARIANT
} = require("./config");
const { writeJson, writeUtf8 } = require("./io");
const {
  buildGcdSteps,
  explainPrimality,
  factorizeCanonical,
  findPrimesInRange,
  findPrimesWithSieve,
  gcdMany,
  normalizeNumbers
} = require("./math");

const ROOT_DIR = path.resolve(__dirname, "..");
const OUTPUT_DIR = path.join(ROOT_DIR, "output");

function parseArgs(argv) {
  const result = { _: [] };

  for (let index = 2; index < argv.length; index++) {
    const current = argv[index];

    if (current.startsWith("--")) {
      const key = current.slice(2);
      const values = [];
      let nextIndex = index + 1;

      while (nextIndex < argv.length && !argv[nextIndex].startsWith("--")) {
        values.push(argv[nextIndex]);
        nextIndex += 1;
      }

      result[key] = values.length <= 1 ? (values[0] ?? true) : values;
      index = nextIndex - 1;
      continue;
    }

    result._.push(current);
  }

  return result;
}

function nowNs() {
  return process.hrtime.bigint();
}

function elapsedMs(start) {
  return Number(process.hrtime.bigint() - start) / 1_000_000;
}

function ensureArray(value) {
  if (Array.isArray(value)) {
    return value;
  }

  if (value === undefined || value === true) {
    return [];
  }

  return [value];
}

function calculateVariantResults() {
  const m = FIRST_NUMBER;
  const n = SECOND_NUMBER;
  const numbers = [m, n];

  const primesToNStart = nowNs();
  const primesToN = findPrimesInRange(2, n);
  const primesToNTimeMs = elapsedMs(primesToNStart);

  const estimateToN = n / Math.log(n);

  const primesRangeStart = nowNs();
  const primesInRange = findPrimesInRange(m, n);
  const primesRangeTimeMs = elapsedMs(primesRangeStart);

  const sieveStart = nowNs();
  const sieveRange = findPrimesWithSieve(m, n);
  const sieveTimeMs = elapsedMs(sieveStart);

  const factorMStart = nowNs();
  const factorM = factorizeCanonical(m);
  const factorMTimeMs = elapsedMs(factorMStart);

  const factorNStart = nowNs();
  const factorN = factorizeCanonical(n);
  const factorNTimeMs = elapsedMs(factorNStart);

  const concatPrimeStart = nowNs();
  const concatenatedCheck = explainPrimality(CONCATENATED_NUMBER);
  const concatPrimeTimeMs = elapsedMs(concatPrimeStart);

  const gcdStart = nowNs();
  const gcdValue = gcdMany(numbers);
  const gcdTimeMs = elapsedMs(gcdStart);

  return {
    variant: VARIANT,
    m,
    n,
    numbers,
    intervalFromTwoToN: {
      range: [2, n],
      count: primesToN.length,
      values: primesToN,
      estimateByPrimeTheorem: Number(estimateToN.toFixed(6)),
      absoluteDifferenceWithEstimate: Number(Math.abs(primesToN.length - estimateToN).toFixed(6)),
      timeMs: Number(primesToNTimeMs.toFixed(6))
    },
    intervalFromMToN: {
      range: [m, n],
      count: primesInRange.length,
      values: primesInRange,
      timeMs: Number(primesRangeTimeMs.toFixed(6)),
      sieveCheck: {
        values: sieveRange.primes,
        matched: JSON.stringify(primesInRange) === JSON.stringify(sieveRange.primes),
        crossedByBasePrime: sieveRange.crossedByBasePrime,
        timeMs: Number(sieveTimeMs.toFixed(6))
      }
    },
    factorization: {
      m: {
        value: m,
        canonical: factorM.canonical,
        steps: factorM.steps,
        timeMs: Number(factorMTimeMs.toFixed(6))
      },
      n: {
        value: n,
        canonical: factorN.canonical,
        steps: factorN.steps,
        timeMs: Number(factorNTimeMs.toFixed(6))
      }
    },
    concatenationPrimeCheck: {
      value: CONCATENATED_NUMBER,
      isPrime: concatenatedCheck.isPrime,
      reason: concatenatedCheck.reason,
      timeMs: Number(concatPrimeTimeMs.toFixed(6))
    },
    gcdResult: {
      input: numbers,
      result: gcdValue,
      steps: buildGcdSteps(numbers),
      timeMs: Number(gcdTimeMs.toFixed(6))
    }
  };
}

function formatCrossedSteps(crossedByBasePrime) {
  return Object.entries(crossedByBasePrime)
    .map(([base, values]) => {
      if (values.length === 0) {
        return `- базовое простое ${base}: новых вычеркиваний нет`;
      }

      return `- базовое простое ${base}: вычеркнуты ${values.join(", ")}`;
    })
    .join("\n");
}

function buildConsoleReport(results) {
  return [
    "Лабораторная работа № 1",
    "Основы теории чисел и их использование в криптографии",
    "",
    `Вариант: ${results.variant}`,
    `m = ${results.m}, n = ${results.n}`,
    "",
    "1. Простые числа на интервале [2, n]",
    `Интервал: [2, ${results.n}]`,
    `Простые числа: ${results.intervalFromTwoToN.values.join(", ")}`,
    `Количество: ${results.intervalFromTwoToN.count}`,
    `Оценка n / ln(n): ${results.intervalFromTwoToN.estimateByPrimeTheorem}`,
    `Модуль разности |pi(n) - n/ln(n)|: ${results.intervalFromTwoToN.absoluteDifferenceWithEstimate}`,
    `Время: ${results.intervalFromTwoToN.timeMs} мс`,
    "",
    "2. Простые числа на интервале [m, n]",
    `Интервал: [${results.m}, ${results.n}]`,
    `Простые числа: ${results.intervalFromMToN.values.join(", ")}`,
    `Количество: ${results.intervalFromMToN.count}`,
    `Время прямой проверки: ${results.intervalFromMToN.timeMs} мс`,
    `Проверка решетом Эратосфена совпала: ${results.intervalFromMToN.sieveCheck.matched ? "да" : "нет"}`,
    `Время решета: ${results.intervalFromMToN.sieveCheck.timeMs} мс`,
    "Этапы решета Эратосфена:",
    formatCrossedSteps(results.intervalFromMToN.sieveCheck.crossedByBasePrime),
    "",
    "3. Каноническое разложение на простые множители",
    `${results.factorization.m.value} = ${results.factorization.m.canonical}`,
    ...results.factorization.m.steps.map((step) => `- ${step}`),
    `Время: ${results.factorization.m.timeMs} мс`,
    `${results.factorization.n.value} = ${results.factorization.n.canonical}`,
    ...results.factorization.n.steps.map((step) => `- ${step}`),
    `Время: ${results.factorization.n.timeMs} мс`,
    "",
    "4. Проверка числа, полученного конкатенацией m || n",
    `Число: ${results.concatenationPrimeCheck.value}`,
    `Простое: ${results.concatenationPrimeCheck.isPrime ? "да" : "нет"}`,
    `Пояснение: ${results.concatenationPrimeCheck.reason}`,
    `Время: ${results.concatenationPrimeCheck.timeMs} мс`,
    "",
    "5. НОД(m, n)",
    `НОД(${results.m}, ${results.n}) = ${results.gcdResult.result}`,
    "Шаги алгоритма Евклида:",
    ...results.gcdResult.steps.map((step) => `- ${step}`),
    `Время: ${results.gcdResult.timeMs} мс`
  ].join("\n");
}

function runAll() {
  const results = calculateVariantResults();
  const summary = {
    lab: "Лабораторная работа № 1. Основы теории чисел и их использование в криптографии",
    variant: VARIANT,
    assumption:
      "Для варианта 9 числа 399 и 433 используются как m и n во всех пунктах практического задания 1.2.",
    results
  };

  writeJson(path.join(OUTPUT_DIR, "summary.json"), summary);
  writeJson(path.join(OUTPUT_DIR, "stats.json"), results);

  const textReport = buildConsoleReport(results);

  writeUtf8(path.join(OUTPUT_DIR, "result.txt"), textReport);
  console.log(textReport);
  console.log("");
  console.log("Артефакты сгенерированы в папке output.");
}

function runGcd(args) {
  const numbers = normalizeNumbers(ensureArray(args.numbers));
  if (numbers.length < 2 || numbers.length > 3) {
    throw new Error("Нужно передать 2 или 3 числа через --numbers.");
  }

  const start = nowNs();
  const result = gcdMany(numbers);
  const timeMs = elapsedMs(start);

  console.log(`Числа: ${numbers.join(", ")}`);
  console.log(`НОД = ${result}`);
  console.log(`Время: ${timeMs.toFixed(6)} мс`);
}

function runPrimes(args) {
  const from = Number(args.from);
  const to = Number(args.to);

  if (!Number.isFinite(from) || !Number.isFinite(to)) {
    throw new Error("Нужно передать --from и --to.");
  }

  const start = nowNs();
  const primes = findPrimesInRange(from, to);
  const timeMs = elapsedMs(start);

  console.log(`Диапазон: [${Math.min(from, to)}, ${Math.max(from, to)}]`);
  console.log(`Простые числа: ${primes.join(", ")}`);
  console.log(`Количество: ${primes.length}`);
  console.log(`Время: ${timeMs.toFixed(6)} мс`);
}

function verify() {
  const results = calculateVariantResults();
  const gcdOk = results.gcdResult.result === 1;
  const expectedPrimes = [401, 409, 419, 421, 431, 433];
  const actualPrimes = results.intervalFromMToN.values;
  const primesOk =
    expectedPrimes.length === actualPrimes.length &&
    expectedPrimes.every((value, index) => value === actualPrimes[index]);
  const sieveOk = results.intervalFromMToN.sieveCheck.matched;
  const factorsOk =
    results.factorization.m.canonical === "3 * 7 * 19" &&
    results.factorization.n.canonical === "433";
  const concatOk = results.concatenationPrimeCheck.isPrime === true;

  console.log(`Проверка НОД: ${gcdOk ? "OK" : "FAIL"}`);
  console.log(`Проверка простых чисел на [m, n]: ${primesOk ? "OK" : "FAIL"}`);
  console.log(`Проверка решета Эратосфена: ${sieveOk ? "OK" : "FAIL"}`);
  console.log(`Проверка факторизации: ${factorsOk ? "OK" : "FAIL"}`);
  console.log(`Проверка числа m||n на простоту: ${concatOk ? "OK" : "FAIL"}`);
}

function printUsage() {
  console.log("Команды:");
  console.log("  node src/cli.js run-all");
  console.log("  node src/cli.js verify");
  console.log("  node src/cli.js gcd --numbers 18 24 42");
  console.log("  node src/cli.js primes --from 10 --to 50");
}

function main() {
  const args = parseArgs(process.argv);
  const command = args._[0];

  if (!command) {
    printUsage();
    process.exitCode = 1;
    return;
  }

  if (command === "run-all") {
    runAll();
    return;
  }

  if (command === "verify") {
    verify();
    return;
  }

  if (command === "gcd") {
    runGcd(args);
    return;
  }

  if (command === "primes") {
    runPrimes(args);
    return;
  }

  printUsage();
  process.exitCode = 1;
}

main();
