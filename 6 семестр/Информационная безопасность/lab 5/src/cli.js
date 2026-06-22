const path = require("path");
const { buildSummary, countFrequencies, toCsvRows } = require("./analysis");
const {
  buildRank,
  decryptMultiple,
  decryptRoute,
  encryptMultiple,
  encryptRoute
} = require("./ciphers");
const { buildHistogramSvg } = require("./histogram");
const { writeJson, writeUtf8, readUtf8 } = require("./io");
const {
  GERMAN_ALPHABET,
  NAME_KEYWORD,
  ROUTE_COLUMNS,
  ROUTE_ROWS,
  SURNAME_KEYWORD
} = require("./config");

const ROOT_DIR = path.resolve(__dirname, "..");
const DEFAULT_SOURCE_FILE = path.join(ROOT_DIR, "data", "source_german.txt");
const OUTPUT_DIR = path.join(ROOT_DIR, "output");

function parseArgs(argv) {
  const result = { _: [] };

  for (let index = 2; index < argv.length; index++) {
    const current = argv[index];

    if (current.startsWith("--")) {
      const key = current.slice(2);
      const next = argv[index + 1];
      result[key] = next && !next.startsWith("--") ? next : true;
      if (result[key] !== true) {
        index += 1;
      }
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

function getCipherTools(cipher) {
  if (cipher === "route") {
    return {
      title: "Маршрутная перестановка (маршрут змейкой)",
      outputName: "route",
      encrypt: encryptRoute,
      decrypt: decryptRoute
    };
  }

  if (cipher === "multiple") {
    return {
      title: "Множественная перестановка",
      outputName: "multiple",
      encrypt: encryptMultiple,
      decrypt: decryptMultiple
    };
  }

  throw new Error(`Неизвестный шифр: ${cipher}`);
}

function createArtifacts(cipher, sourcePath) {
  const tools = getCipherTools(cipher);
  const plainText = readUtf8(sourcePath);

  const encryptStart = nowNs();
  const encryption = tools.encrypt(plainText);
  const encryptTimeMs = elapsedMs(encryptStart);

  const decryptStart = nowNs();
  const decryptedText = tools.decrypt(encryption.output, encryption.originalLength);
  const decryptTimeMs = elapsedMs(decryptStart);

  const outputDir = path.join(OUTPUT_DIR, tools.outputName);
  const originalStats = countFrequencies(plainText);
  const encryptedStats = countFrequencies(encryption.output);
  const decryptedStats = countFrequencies(decryptedText);
  const originalSummary = buildSummary(originalStats);
  const encryptedSummary = buildSummary(encryptedStats);
  const decryptedSummary = buildSummary(decryptedStats);

  writeUtf8(path.join(outputDir, "source.txt"), plainText);
  writeUtf8(path.join(outputDir, "encrypted.txt"), encryption.output);
  writeUtf8(path.join(outputDir, "decrypted.txt"), decryptedText);
  writeUtf8(path.join(outputDir, "freq_original.csv"), toCsvRows(originalStats));
  writeUtf8(path.join(outputDir, "freq_encrypted.csv"), toCsvRows(encryptedStats));
  writeUtf8(path.join(outputDir, "freq_decrypted.csv"), toCsvRows(decryptedStats));
  writeUtf8(
    path.join(outputDir, "histogram_original.svg"),
    buildHistogramSvg("Гистограмма исходного текста", originalStats)
  );
  writeUtf8(
    path.join(outputDir, "histogram_encrypted.svg"),
    buildHistogramSvg("Гистограмма шифрованного текста", encryptedStats)
  );
  writeUtf8(
    path.join(outputDir, "histogram_decrypted.svg"),
    buildHistogramSvg("Гистограмма расшифрованного текста", decryptedStats)
  );

  const stats = {
    cipher: tools.title,
    sourceFile: sourcePath,
    alphabet: GERMAN_ALPHABET,
    sourceLength: plainText.length,
    encryptedLength: Array.from(encryption.output).length,
    onlyGermanLetters: originalStats.total,
    paddingLength: encryption.paddingLength,
    encryptionTimeMs: Number(encryptTimeMs.toFixed(6)),
    decryptionTimeMs: Number(decryptTimeMs.toFixed(6)),
    decryptedMatchesSource: decryptedText === plainText,
    originalSummary,
    encryptedSummary,
    decryptedSummary
  };

  writeJson(path.join(outputDir, "stats.json"), stats);
  return stats;
}

function writeSummary(results) {
  const summary = {
    lab: "Лабораторная работа № 3. Исследование криптографических шифров на основе перестановки символов",
    variant: 4,
    language: "Немецкий",
    assumptions: {
      routeTable: {
        rows: ROUTE_ROWS,
        columns: ROUTE_COLUMNS,
        note: "Параметры таблицы выбраны как рабочее допущение, так как в задании отсутствует отдельное указание ключа."
      },
      multipleTransposition: {
        rowKeyword: NAME_KEYWORD,
        columnKeyword: SURNAME_KEYWORD,
        rowOrder: buildRank(NAME_KEYWORD),
        columnOrder: buildRank(SURNAME_KEYWORD)
      }
    },
    results
  };

  writeJson(path.join(OUTPUT_DIR, "summary.json"), summary);
}

function verify(sourcePath) {
  const source = readUtf8(sourcePath);
  const routeEncrypted = encryptRoute(source);
  const multipleEncrypted = encryptMultiple(source);
  const routeOk = decryptRoute(routeEncrypted.output, routeEncrypted.originalLength) === source;
  const multipleOk =
    decryptMultiple(multipleEncrypted.output, multipleEncrypted.originalLength) === source;

  console.log(`Источник: ${sourcePath}`);
  console.log(`Длина текста: ${source.length}`);
  console.log(`Проверка маршрутной перестановки: ${routeOk ? "OK" : "FAIL"}`);
  console.log(`Проверка множественной перестановки: ${multipleOk ? "OK" : "FAIL"}`);
}

function printUsage() {
  console.log("Команды:");
  console.log("  node src/cli.js run-all [--input data/source_german.txt]");
  console.log("  node src/cli.js verify [--input data/source_german.txt]");
  console.log("  node src/cli.js encrypt --cipher route|multiple [--input file]");
  console.log("  node src/cli.js decrypt --cipher route|multiple [--input file]");
}

function main() {
  const args = parseArgs(process.argv);
  const command = args._[0];
  const inputPath = path.resolve(args.input || DEFAULT_SOURCE_FILE);

  if (!command) {
    printUsage();
    process.exitCode = 1;
    return;
  }

  if (command === "run-all") {
    const routeStats = createArtifacts("route", inputPath);
    const multipleStats = createArtifacts("multiple", inputPath);
    writeSummary({ route: routeStats, multiple: multipleStats });
    console.log("Артефакты сгенерированы в папке output.");
    return;
  }

  if (command === "verify") {
    verify(inputPath);
    return;
  }

  if (command === "encrypt" || command === "decrypt") {
    const cipher = args.cipher;
    if (!cipher) {
      throw new Error("Нужно указать --cipher route или --cipher multiple");
    }

    const tools = getCipherTools(cipher);
    const source = readUtf8(inputPath);

    if (command === "encrypt") {
      const encryption = tools.encrypt(source);
      console.log(encryption.output);
      return;
    }

    const encryption = tools.encrypt(source);
    console.log(tools.decrypt(encryption.output, encryption.originalLength));
    return;
  }

  printUsage();
  process.exitCode = 1;
}

main();
