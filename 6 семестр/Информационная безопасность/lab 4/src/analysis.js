const { GERMAN_ALPHABET } = require("./config");

function filterGermanLetters(text) {
  return Array.from(text.toLowerCase()).filter((char) =>
    GERMAN_ALPHABET.includes(char)
  );
}

function countFrequencies(text) {
  const letters = filterGermanLetters(text);
  const counts = Object.fromEntries(
    Array.from(GERMAN_ALPHABET).map((char) => [char, 0])
  );

  for (const char of letters) {
    counts[char] += 1;
  }

  const total = letters.length;
  const frequencies = Array.from(GERMAN_ALPHABET).map((char) => ({
    symbol: char,
    count: counts[char],
    frequency: total === 0 ? 0 : counts[char] / total
  }));

  return { total, frequencies };
}

function toCsvRows(stats) {
  const lines = ["Символ,Количество,Частота"];

  for (const item of stats.frequencies) {
    lines.push(
      `${item.symbol},${item.count},${item.frequency.toFixed(6)}`
    );
  }

  return lines.join("\n");
}

function buildSummary(stats) {
  const sorted = [...stats.frequencies].sort((left, right) => {
    if (right.count !== left.count) {
      return right.count - left.count;
    }

    return left.symbol.localeCompare(right.symbol, "de");
  });

  return {
    totalLetters: stats.total,
    top10: sorted.slice(0, 10)
  };
}

module.exports = {
  buildSummary,
  countFrequencies,
  toCsvRows
};
