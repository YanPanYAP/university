const path = require("path");
const { buildSummary, countFrequencies, normalizeText, toCsvRows } = require("./analysis");
const {
  DEFAULT_RING_SETTINGS,
  MESSAGE,
  TASK1_SETTINGS,
  VARIANT
} = require("./config");
const { decrypt, encrypt } = require("./enigma");
const { ensureDir, writeJson, writeUtf8 } = require("./io");

const ROOT_DIR = path.resolve(__dirname, "..");
const OUTPUT_DIR = path.join(ROOT_DIR, "output");

function formatTrace(trace) {
  const forward = trace.path.forward
    .map(
      (step) =>
        `${step.rotor}:${step.input}->${step.shifted}->${step.wired}->${step.output}`
    )
    .join(" | ");
  const backward = trace.path.backward
    .map(
      (step) =>
        `${step.rotor}:${step.input}->${step.shifted}->${step.wired}->${step.output}`
    )
    .join(" | ");

  return `${trace.step}. pos=${trace.position}; ${trace.input} -> ${forward} | REF:${trace.path.reflector.input}->${trace.path.reflector.output} | ${backward} -> ${trace.output}`;
}

function runSetting(initialPosition) {
  const plaintext = normalizeText(MESSAGE.normalized);
  const encryption = encrypt(plaintext, {
    initialPosition,
    ringSettings: DEFAULT_RING_SETTINGS
  });
  const decryption = decrypt(encryption.ciphertext, {
    initialPosition,
    ringSettings: DEFAULT_RING_SETTINGS
  });
  const stats = countFrequencies(encryption.ciphertext);

  return {
    initialPosition,
    finalPosition: encryption.finalPosition,
    plaintext,
    ciphertext: encryption.ciphertext,
    encryptionTraces: encryption.traces,
    decryptedText: decryption.plaintext,
    decryptionTraces: decryption.traces,
    decryptedMatches: decryption.plaintext === plaintext,
    summary: buildSummary(stats)
  };
}

function generateArtifacts() {
  ensureDir(OUTPUT_DIR);

  const sourceStats = countFrequencies(MESSAGE.normalized);
  const sourceSummary = buildSummary(sourceStats);
  const task1Runs = TASK1_SETTINGS.map(runSetting);

  writeUtf8(path.join(OUTPUT_DIR, "source.txt"), `${MESSAGE.original}\n${MESSAGE.normalized}\n`);
  writeUtf8(path.join(OUTPUT_DIR, "source_frequency.csv"), toCsvRows(sourceStats));

  for (const run of task1Runs) {
    writeUtf8(
      path.join(OUTPUT_DIR, "task1", `${run.initialPosition}.txt`),
      `plaintext=${run.plaintext}\nciphertext=${run.ciphertext}\nencrypt_traces=\n${run.encryptionTraces.map(formatTrace).join("\n")}\ndecrypted=${run.decryptedText}\nmatch=${run.decryptedMatches}\ndecrypt_traces=\n${run.decryptionTraces.map(formatTrace).join("\n")}\n`
    );
    writeUtf8(
      path.join(OUTPUT_DIR, "task1", `${run.initialPosition}.csv`),
      toCsvRows(countFrequencies(run.ciphertext))
    );
  }

  const summary = {
    lab: "Лабораторная работа № 6. Шифровальная машина Энигма",
    variant: VARIANT.number,
    machine: {
      leftRotor: VARIANT.leftRotor,
      middleRotor: VARIANT.middleRotor,
      rightRotor: VARIANT.rightRotor,
      reflector: VARIANT.reflector,
      stepPattern: "1-0-1",
      ringSettings: "AAA"
    },
    message: MESSAGE,
    sourceSummary,
    task1: {
      simulator: "M3-совместимая конфигурация",
      settingsCount: task1Runs.length,
      results: task1Runs
    }
  };

  writeJson(path.join(OUTPUT_DIR, "summary.json"), summary);
  writeUtf8(path.join(ROOT_DIR, "lab6.txt"), "npm run run-all\nnpm run verify\n");
  writeUtf8(path.join(ROOT_DIR, "report.txt"), buildReport(summary));
}

function buildReport(summary) {
  const lines = [
    "Лабораторная работа № 6",
    "Тема: моделирование шифровальной машины Энигма",
    "",
    `Вариант: ${summary.variant}`,
    `Роторы: ${summary.machine.leftRotor}-${summary.machine.middleRotor}-${summary.machine.rightRotor}`,
    `Отражатель: ${summary.machine.reflector}`,
    `Схема сдвига: ${summary.machine.stepPattern}`,
    `Начальные кольцевые установки: ${summary.machine.ringSettings}`,
    "",
    `Исходное сообщение: ${summary.message.original}`,
    `Нормализованное сообщение: ${summary.message.normalized}`,
    "",
    "Частотные свойства исходного текста:",
    `Индекс совпадений: ${summary.sourceSummary.indexOfCoincidence.toFixed(6)}`,
    `Энтропия Шеннона: ${summary.sourceSummary.shannonEntropy.toFixed(6)}`,
    `Топ-10 символов: ${formatTop(summary.sourceSummary.top10)}`,
    "",
    "Пункт 1. Шифрование при 10 различных настройках:",
    ...summary.task1.results.flatMap((result) => [
      `${result.initialPosition} -> ${result.ciphertext}; IC=${result.summary.indexOfCoincidence.toFixed(6)}; H=${result.summary.shannonEntropy.toFixed(6)}`,
      "  Пути шифрования:",
      ...result.encryptionTraces.map((trace) => `    ${formatTrace(trace)}`),
      `  decrypt=${result.decryptedText}; match=${result.decryptedMatches};`,
      "  Пути дешифрования:",
      ...result.decryptionTraces.map((trace) => `    ${formatTrace(trace)}`)
    ])
  ];

  return `${lines.join("\n")}\n`;
}

function formatTop(top10) {
  return top10.map((item) => `${item.symbol}:${item.count}`).join(", ");
}

function verify() {
  const known = encrypt("AAAAAAAAAA", {
    initialPosition: "AAA",
    ringSettings: DEFAULT_RING_SETTINGS,
    rotorOrder: {
      left: "I",
      middle: "II",
      right: "III"
    },
    stepPattern: {
      left: 0,
      middle: 0,
      right: 1
    }
  });

  console.log(`Тестовый шифртекст для I-II-III/B/AAA: ${known.ciphertext}`);
  console.log(`Ожидаемое начало: BDZGOWCXLT`);
  console.log(
    `Проверка: ${known.ciphertext === "BDZGOWCXLT" ? "OK" : "FAIL"}`
  );
}

function parseArgs(argv) {
  return argv[2];
}

function main() {
  const command = parseArgs(process.argv);

  if (command === "run-all") {
    generateArtifacts();
    console.log("Артефакты лабораторной работы сформированы в папке output.");
    return;
  }

  if (command === "verify") {
    verify();
    return;
  }

  console.log("Команды:");
  console.log("  npm run run-all");
  console.log("  npm run verify");
  process.exitCode = 1;
}

main();
