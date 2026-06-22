const fs = require("fs");

function parseArgs(argv) {
    const args = {};
    for (let i = 2; i < argv.length; i++) {
        if (argv[i].startsWith("--")) {
            const key = argv[i].slice(2);
            const value = argv[i + 1] && !argv[i + 1].startsWith("--")
                ? argv[i + 1]
                : true;
            args[key] = value;
            if (value !== true) i++;
        }
    }
    return args;
}

function normalizeText(text, alphabet) {
    text = text.toLowerCase();

    const alphabets = {
        latin: {
            regex: /[a-z]/g,
            size: 26
        },
        dutch: {
            regex: /[a-záéëïóúü]/g,
            size: 26
        },
        ukrainian: {
            regex: /[а-щьюяіїєґ]/g,
            size: 33
        },
        russian: {
            regex: /[а-яё]/gi,
            size: 33
}
    };

    if (!alphabet || !alphabets[alphabet]) {
        return { cleanText: text.replace(/\s/g, ""), alphabetSize: null };
    }

    const matches = text.match(alphabets[alphabet].regex);
    const cleanText = matches ? matches.join("") : "";

    return {
        cleanText,
        alphabetSize: alphabets[alphabet].size
    };
}

function getFrequencies(text) {
    const freq = {};
    for (const ch of text) {
        freq[ch] = (freq[ch] || 0) + 1;
    }
    return freq;
}

function calculateEntropy(freq) {
    let H = 0;
    const total = Object.values(freq).reduce((a, b) => a + b, 0);

    for (const count of Object.values(freq)) {
        const p = count / total;
        H -= p * Math.log2(p);
    }

    return H;
}

function effectiveEntropy(p) {
    if (p === 0) return 1;
    if (p === 0.5) return 0;
    if (p === 1) return 1;

    return 1 + p * Math.log2(p) + (1 - p) * Math.log2(1 - p);
}

function printChannelErrorImpact(information, label = "I_eff") {
    console.log("\nС учетом ошибок канала:");

    [0.1, 0.5, 1].forEach((p) => {
        const He = effectiveEntropy(p);
        const Ieff = He * information;
        console.log(`p=${p} → H_e=${He.toFixed(6)} → ${label} = ${Ieff.toFixed(6)} бит`);
    });
}

function analyzeText(text, alphabetSize, csvFileName = "output.csv") {
    const freq = getFrequencies(text);
    const total = Object.values(freq).reduce((a, b) => a + b, 0);

    console.log(`Общее число символов N = ${total}\n`);
    console.log("Символ | Частота n_i | Вероятность p_i | Бинарный код");
    console.log("----------------------------------------");

    let csvContent = "Символ,Частота n_i,Вероятность p_i,Бинарный код\n";

    for (const [symbol, count] of Object.entries(freq)) {
        const p = count / total;
        const binary = symbol.charCodeAt(0).toString(2).padStart(16, "0");
        console.log(`${symbol} | ${count} | ${p.toFixed(6)} | ${binary}`);
        csvContent += `${symbol},${count},${p.toFixed(6)},${binary}\n`;
    }

    const entropy = calculateEntropy(freq);
    console.log(`\nЭнтропия алфавита H = ${entropy.toFixed(6)} бит/символ`);

    const informationAlphabet = entropy * total;
    const informationASCII = 8 * total;

    console.log(`\nКоличество информации на основе энтропии алфавита I = H × N = ${informationAlphabet.toFixed(6)} бит`);
    console.log(`Количество информации в кодах ASCII I_ASCII = 8 × N = ${informationASCII.toFixed(6)} бит`);

    if (alphabetSize) {
        const Hmax = Math.log2(alphabetSize);
        const redundancy = 1 - entropy / Hmax;
        console.log(`Максимальная энтропия H_max = log2(${alphabetSize}) = ${Hmax.toFixed(6)}`);
        console.log(`Относительная избыточность R = ${redundancy.toFixed(6)}`);
    }

    console.log("\nС учетом ошибок канала:");
    [0.1, 0.5, 1].forEach((p) => {
        const He = effectiveEntropy(p);
        const IeffAlphabet = He * informationAlphabet;
        const IeffASCII = He * informationASCII;
        console.log(`p=${p} → H_e=${He.toFixed(6)} → I_eff (алфавит) = ${IeffAlphabet.toFixed(6)} бит, I_eff (ASCII) = ${IeffASCII.toFixed(6)} бит`);
    });

    fs.writeFileSync(csvFileName, csvContent, "utf8");
}

function analyzeTextBinaryEntropy(text) {
    const buffer = Buffer.from(text, "utf8");
    const totalBits = buffer.length * 8;
    let ones = 0;

    for (const byte of buffer) {
        let b = byte;
        for (let i = 0; i < 8; i++) {
            if (b & 1) ones++;
            b >>= 1;
        }
    }

    const zeros = totalBits - ones;
    const p1 = ones / totalBits;
    const p0 = zeros / totalBits;

    const H =
        p1 === 0 || p1 === 1
            ? 0
            : -p1 * Math.log2(p1) - p0 * Math.log2(p0);

    const information = H * totalBits;

    console.log("\n--- Бинарная энтропия текста ---");
    console.log(`Всего бит: ${totalBits}`);
    console.log(`Количество единиц: ${ones}`);
    console.log(`Количество нулей: ${zeros}`);
    console.log(`p(1) = ${p1.toFixed(6)}, p(0) = ${p0.toFixed(6)}`);
    console.log(`Энтропия бинарного алфавита H = ${H.toFixed(6)} бит/бит`);
    console.log(`Количество информации I = H × totalBits = ${information.toFixed(6)} бит`);
    printChannelErrorImpact(information, "I_eff (бинарный текст)");
}

function analyzeBinaryFile(path) {
    const buffer = fs.readFileSync(path);

    const totalBits = buffer.length * 8;
    let ones = 0;

    for (const byte of buffer) {
        let b = byte;
        for (let i = 0; i < 8; i++) {
            if (b & 1) ones++;
            b >>= 1;
        }
    }

    const zeros = totalBits - ones;
    const p1 = ones / totalBits;
    const p0 = zeros / totalBits;

    const H =
        p1 === 0 || p1 === 1
            ? 0
            : -p1 * Math.log2(p1) - p0 * Math.log2(p0);

    const information = H * totalBits;

    console.log("\n--- Бинарная энтропия файла ---");
    console.log(`Всего бит: ${totalBits}`);
    console.log(`Количество единиц: ${ones}`);
    console.log(`Количество нулей: ${zeros}`);
    console.log(`p(1) = ${p1.toFixed(6)}, p(0) = ${p0.toFixed(6)}`);
    console.log(`Энтропия бинарного алфавита H = ${H.toFixed(6)} бит/бит`);
    console.log(`Количество информации I = H × totalBits = ${information.toFixed(6)} бит`);
    printChannelErrorImpact(information, "I_eff (бинарный файл)");
}

function main() {
    const args = parseArgs(process.argv);

    if (args.file) {
    console.log("=== АНАЛИЗ ТЕКСТОВОГО ФАЙЛА ===\n");
    const rawText = fs.readFileSync(args.file, "utf8");

    const cleanText = rawText;
    analyzeText(cleanText, null);
    analyzeTextBinaryEntropy(cleanText);
    return;
}

if (args.text) {
    console.log("=== АНАЛИЗ СТРОКИ ===\n");
    const cleanText = args.text;
    analyzeText(cleanText, null);
    analyzeTextBinaryEntropy(cleanText);
    return;
}

    if (args["binary-file"]) {
        console.log("=== АНАЛИЗ БИНАРНОГО ФАЙЛА ===\n");
        analyzeBinaryFile(args["binary-file"]);
        return;
    }

    console.log("Не указаны входные данные.");
}

main();
