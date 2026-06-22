const { ALPHABET } = require("./config");

function normalizeText(text) {
  return Array.from(text.toUpperCase())
    .filter((char) => ALPHABET.includes(char))
    .join("");
}

function countFrequencies(text) {
  const normalized = normalizeText(text);
  const counts = Object.fromEntries(Array.from(ALPHABET).map((char) => [char, 0]));

  for (const char of normalized) {
    counts[char] += 1;
  }

  const total = normalized.length;
  const frequencies = Array.from(ALPHABET).map((char) => ({
    symbol: char,
    count: counts[char],
    frequency: total === 0 ? 0 : counts[char] / total
  }));

  return { total, frequencies };
}

function buildSummary(stats) {
  const top10 = [...stats.frequencies]
    .sort((left, right) => {
      if (right.count !== left.count) {
        return right.count - left.count;
      }

      return left.symbol.localeCompare(right.symbol, "en");
    })
    .slice(0, 10);

  return {
    totalLetters: stats.total,
    indexOfCoincidence: calculateIndexOfCoincidence(stats),
    shannonEntropy: calculateEntropy(stats),
    top10
  };
}

function calculateIndexOfCoincidence(stats) {
  if (stats.total < 2) {
    return 0;
  }

  const numerator = stats.frequencies.reduce(
    (sum, item) => sum + item.count * (item.count - 1),
    0
  );

  return numerator / (stats.total * (stats.total - 1));
}

function calculateEntropy(stats) {
  return stats.frequencies.reduce((sum, item) => {
    if (item.frequency === 0) {
      return sum;
    }

    return sum - item.frequency * Math.log2(item.frequency);
  }, 0);
}

function toCsvRows(stats) {
  const rows = ["Символ,Количество,Частота"];

  for (const item of stats.frequencies) {
    rows.push(`${item.symbol},${item.count},${item.frequency.toFixed(6)}`);
  }

  return rows.join("\n");
}

module.exports = {
  buildSummary,
  countFrequencies,
  normalizeText,
  toCsvRows
};
