const {
  GERMAN_ALPHABET,
  SHIFT_K,
  TRISEMUS_KEYWORD,
  TRISEMUS_COLUMNS
} = require("./config");

const UPPERCASE_MAP = {
  ä: "Ä",
  ö: "Ö",
  ü: "Ü",
  ß: "ẞ"
};

function uniqueChars(value) {
  const seen = new Set();
  let result = "";

  for (const char of value) {
    if (!seen.has(char) && GERMAN_ALPHABET.includes(char)) {
      seen.add(char);
      result += char;
    }
  }

  return result;
}

function buildTrisemusSequence() {
  const keyword = uniqueChars(TRISEMUS_KEYWORD.toLowerCase());
  let sequence = keyword;

  for (const char of GERMAN_ALPHABET) {
    if (!sequence.includes(char)) {
      sequence += char;
    }
  }

  return sequence;
}

function buildTrisemusTable() {
  const sequence = buildTrisemusSequence();
  const table = [];

  for (let index = 0; index < sequence.length; index += TRISEMUS_COLUMNS) {
    table.push(sequence.slice(index, index + TRISEMUS_COLUMNS).split(""));
  }

  return table;
}

function applyOriginalCase(sourceChar, replacement) {
  if (sourceChar === sourceChar.toLowerCase()) {
    return replacement;
  }

  return UPPERCASE_MAP[replacement] || replacement.toUpperCase();
}

function transformByShift(text, direction) {
  const normalizedDirection = direction === "decrypt" ? -SHIFT_K : SHIFT_K;
  let result = "";

  for (const char of text) {
    const lower = char.toLowerCase();
    const index = GERMAN_ALPHABET.indexOf(lower);

    if (index === -1) {
      result += char;
      continue;
    }

    const nextIndex =
      (index + normalizedDirection + GERMAN_ALPHABET.length) %
      GERMAN_ALPHABET.length;
    const replacement = GERMAN_ALPHABET[nextIndex];
    result += applyOriginalCase(char, replacement);
  }

  return result;
}

function buildTrisemusMaps() {
  const table = buildTrisemusTable();
  const encryptMap = new Map();
  const decryptMap = new Map();

  for (let row = 0; row < table.length; row++) {
    for (let column = 0; column < table[row].length; column++) {
      const source = table[row][column];
      const target = table[(row + 1) % table.length][column];
      const reverse = table[(row - 1 + table.length) % table.length][column];
      encryptMap.set(source, target);
      decryptMap.set(source, reverse);
    }
  }

  return { table, encryptMap, decryptMap };
}

function transformByTrisemus(text, direction) {
  const { encryptMap, decryptMap } = buildTrisemusMaps();
  const dictionary = direction === "decrypt" ? decryptMap : encryptMap;
  let result = "";

  for (const char of text) {
    const lower = char.toLowerCase();
    const replacement = dictionary.get(lower);

    if (!replacement) {
      result += char;
      continue;
    }

    result += applyOriginalCase(char, replacement);
  }

  return result;
}

function encryptShift(text) {
  return transformByShift(text, "encrypt");
}

function decryptShift(text) {
  return transformByShift(text, "decrypt");
}

function encryptTrisemus(text) {
  return transformByTrisemus(text, "encrypt");
}

function decryptTrisemus(text) {
  return transformByTrisemus(text, "decrypt");
}

module.exports = {
  buildTrisemusSequence,
  buildTrisemusTable,
  decryptShift,
  decryptTrisemus,
  encryptShift,
  encryptTrisemus
};
