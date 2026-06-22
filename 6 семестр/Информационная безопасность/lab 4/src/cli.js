const path = require("path");
const {
  buildTrisemusSequence,
  buildTrisemusTable,
  decryptShift,
  decryptTrisemus,
  encryptShift,
  encryptTrisemus
} = require("./ciphers");
const { buildSummary, countFrequencies, toCsvRows } = require("./analysis");
const { buildHistogramSvg } = require("./histogram");
const { readUtf8, writeJson, writeUtf8 } = require("./io");
const { GERMAN_ALPHABET, SHIFT_K, TRISEMUS_KEYWORD } = require("./config");

const ROOT_DIR = path.resolve(__dirname, "..");
const DEFAULT_SOURCE_FILE = path.join(ROOT_DIR, "data", "source_german.txt");
const OUTPUT_DIR = path.join(ROOT_DIR, "output");

function parseArgs(argv) {
  const result = { _: [] };

  for (let index = 2; index < argv.length; index++) {
    const value = argv[index];

    if (value.startsWith("--")) {
      const key = value.slice(2);
      const next = argv[index + 1];
      result[key] = next && !next.startsWith("--") ? next : true;
      if (result[key] !== true) {
        index += 1;
      }
      continue;
    }

    result._.push(value);
  }

  return result;
}

function nowNs() {
  return process.hrtime.bigint();
}

function elapsedMs(start) {
  return Number(nowNs() - start) / 1_000_000;
}

function getCipherTools(cipher) {
  if (cipher === "shift") {
    return {
      title: "Шифр замены по формулам (2.1) и (2.2)",
      outputName: "shift",
      encrypt: encryptShift,
      decrypt: decryptShift
    };
  }

  if (cipher === "trisemus") {
    return {
      title: "Шифр таблицы Трисемуса",
      outputName: "trisemus",
      encrypt: encryptTrisemus,
      decrypt: decryptTrisemus
    };
  }

  throw new Error(`Неизвестный шифр: ${cipher}`);
}

function createArtifacts(cipher, sourcePath) {
  const tools = getCipherTools(cipher);
  const plainText = readUtf8(sourcePath);
  const encryptStart = nowNs();
  const encryptedText = tools.encrypt(plainText);
  const encryptTimeMs = elapsedMs(encryptStart);

  const decryptStart = nowNs();
  const decryptedText = tools.decrypt(encryptedText);
  const decryptTimeMs = elapsedMs(decryptStart);

  const outputDir = path.join(OUTPUT_DIR, tools.outputName);
  const originalStats = countFrequencies(plainText);
  const encryptedStats = countFrequencies(encryptedText);
  const decryptedStats = countFrequencies(decryptedText);
  const originalSummary = buildSummary(originalStats);
  const encryptedSummary = buildSummary(encryptedStats);
  const decryptedSummary = buildSummary(decryptedStats);

  writeUtf8(path.join(outputDir, "source.txt"), plainText);
  writeUtf8(path.join(outputDir, "encrypted.txt"), encryptedText);
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
    onlyGermanLetters: originalStats.total,
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

function writeLabSummary(results) {
  const summaryPath = path.join(OUTPUT_DIR, "summary.json");
  const content = {
    lab: "Лабораторная работа № 2. Исследование криптографических шифров на основе подстановки (замены) символов",
    variant: 9,
    language: "Немецкий",
    formulaCipher: {
      method: "Сдвиговая подстановка по формулам (2.1) и (2.2)",
      k: SHIFT_K
    },
    trisemusCipher: {
      method: "Таблица Трисемуса",
      keyword: TRISEMUS_KEYWORD,
      sequence: buildTrisemusSequence(),
      table: buildTrisemusTable()
    },
    results
  };

  writeJson(summaryPath, content);
}

function verify(sourcePath) {
  const source = readUtf8(sourcePath);
  const shiftEncrypted = encryptShift(source);
  const trisemusEncrypted = encryptTrisemus(source);
  const shiftOk = decryptShift(shiftEncrypted) === source;
  const trisemusOk = decryptTrisemus(trisemusEncrypted) === source;

  console.log(`Источник: ${sourcePath}`);
  console.log(`Длина текста: ${source.length}`);
  console.log(`Проверка шифра по формулам (2.1), (2.2): ${shiftOk ? "OK" : "FAIL"}`);
  console.log(`Проверка таблицы Трисемуса: ${trisemusOk ? "OK" : "FAIL"}`);
}

function printUsage() {
  console.log("Команды:");
  console.log("  node src/cli.js run-all [--input data/source_german.txt]");
  console.log("  node src/cli.js encrypt --cipher shift|trisemus [--input file]");
  console.log("  node src/cli.js decrypt --cipher shift|trisemus [--input file]");
  console.log("  node src/cli.js verify [--input data/source_german.txt]");
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
    const shiftStats = createArtifacts("shift", inputPath);
    const trisemusStats = createArtifacts("trisemus", inputPath);
    writeLabSummary({ shift: shiftStats, trisemus: trisemusStats });
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
      throw new Error("Нужно указать --cipher shift или --cipher trisemus");
    }

    const tools = getCipherTools(cipher);
    const source = readUtf8(inputPath);
    const result =
      command === "encrypt" ? tools.encrypt(source) : tools.decrypt(source);
    console.log(result);
    return;
  }

  printUsage();
  process.exitCode = 1;
}

main();
