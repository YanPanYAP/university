const {
  NAME_KEYWORD,
  ROUTE_COLUMNS,
  ROUTE_ROWS,
  SURNAME_KEYWORD
} = require("./config");

function splitToChars(value) {
  return Array.from(value);
}

function repeatPadding(source, targetLength) {
  const chars = splitToChars(source);
  let result = source;
  let index = 0;

  while (splitToChars(result).length < targetLength) {
    result += chars[index % chars.length];
    index += 1;
  }

  return result;
}

function padBlock(text, blockSize) {
  const originalLength = splitToChars(text).length;
  const remainder = originalLength % blockSize;
  const paddingLength = remainder === 0 ? 0 : blockSize - remainder;
  const paddedText =
    paddingLength === 0
      ? text
      : repeatPadding(text, originalLength + paddingLength);

  return {
    paddedText,
    paddingLength,
    originalLength
  };
}

function sliceBlocks(text, blockSize) {
  const chars = splitToChars(text);
  const blocks = [];

  for (let index = 0; index < chars.length; index += blockSize) {
    blocks.push(chars.slice(index, index + blockSize));
  }

  return blocks;
}

function createEmptyMatrix(rows, columns) {
  return Array.from({ length: rows }, () => Array(columns).fill(""));
}

function fillMatrixRowWise(chars, rows, columns) {
  const matrix = createEmptyMatrix(rows, columns);
  let index = 0;

  for (let row = 0; row < rows; row++) {
    for (let column = 0; column < columns; column++) {
      matrix[row][column] = chars[index];
      index += 1;
    }
  }

  return matrix;
}

function readMatrixRowWise(matrix) {
  return matrix.flat().join("");
}

function readMatrixSnakeByColumns(matrix) {
  const rows = matrix.length;
  const columns = matrix[0].length;
  const result = [];

  for (let column = 0; column < columns; column++) {
    if (column % 2 === 0) {
      for (let row = 0; row < rows; row++) {
        result.push(matrix[row][column]);
      }
    } else {
      for (let row = rows - 1; row >= 0; row--) {
        result.push(matrix[row][column]);
      }
    }
  }

  return result.join("");
}

function fillMatrixSnakeByColumns(chars, rows, columns) {
  const matrix = createEmptyMatrix(rows, columns);
  let index = 0;

  for (let column = 0; column < columns; column++) {
    if (column % 2 === 0) {
      for (let row = 0; row < rows; row++) {
        matrix[row][column] = chars[index];
        index += 1;
      }
    } else {
      for (let row = rows - 1; row >= 0; row--) {
        matrix[row][column] = chars[index];
        index += 1;
      }
    }
  }

  return matrix;
}

function encryptRouteBlock(blockChars) {
  const matrix = fillMatrixRowWise(blockChars, ROUTE_ROWS, ROUTE_COLUMNS);
  return readMatrixSnakeByColumns(matrix);
}

function decryptRouteBlock(blockChars) {
  const matrix = fillMatrixSnakeByColumns(blockChars, ROUTE_ROWS, ROUTE_COLUMNS);
  return readMatrixRowWise(matrix);
}

function buildRank(keyword) {
  const chars = splitToChars(keyword.toLowerCase());
  return chars
    .map((char, index) => ({ char, index }))
    .sort((left, right) => {
      const comparison = left.char.localeCompare(right.char, "de");
      if (comparison !== 0) {
        return comparison;
      }

      return left.index - right.index;
    })
    .map((item) => item.index);
}

function permuteColumns(matrix, order) {
  return matrix.map((row) => order.map((sourceIndex) => row[sourceIndex]));
}

function invertColumnPermutation(matrix, order) {
  return matrix.map((row) => {
    const restored = Array(order.length);
    for (let index = 0; index < order.length; index++) {
      restored[order[index]] = row[index];
    }
    return restored;
  });
}

function permuteRows(matrix, order) {
  return order.map((sourceIndex) => matrix[sourceIndex].slice());
}

function invertRowPermutation(matrix, order) {
  const restored = Array(order.length);
  for (let index = 0; index < order.length; index++) {
    restored[order[index]] = matrix[index].slice();
  }
  return restored;
}

function encryptMultipleBlock(blockChars) {
  const rows = splitToChars(NAME_KEYWORD).length;
  const columns = splitToChars(SURNAME_KEYWORD).length;
  const matrix = fillMatrixRowWise(blockChars, rows, columns);
  const columnOrder = buildRank(SURNAME_KEYWORD);
  const rowOrder = buildRank(NAME_KEYWORD);
  const afterColumns = permuteColumns(matrix, columnOrder);
  const afterRows = permuteRows(afterColumns, rowOrder);
  return readMatrixRowWise(afterRows);
}

function decryptMultipleBlock(blockChars) {
  const rows = splitToChars(NAME_KEYWORD).length;
  const columns = splitToChars(SURNAME_KEYWORD).length;
  const matrix = fillMatrixRowWise(blockChars, rows, columns);
  const columnOrder = buildRank(SURNAME_KEYWORD);
  const rowOrder = buildRank(NAME_KEYWORD);
  const afterRows = invertRowPermutation(matrix, rowOrder);
  const afterColumns = invertColumnPermutation(afterRows, columnOrder);
  return readMatrixRowWise(afterColumns);
}

function processBlocks(text, blockSize, transform) {
  const { paddedText, paddingLength, originalLength } = padBlock(text, blockSize);
  const blocks = sliceBlocks(paddedText, blockSize);
  const transformed = blocks
    .map((block) => transform(block).toString())
    .join("");

  return {
    output: transformed,
    paddingLength,
    originalLength
  };
}

function trimPadding(text, originalLength) {
  return splitToChars(text).slice(0, originalLength).join("");
}

function encryptRoute(text) {
  const blockSize = ROUTE_ROWS * ROUTE_COLUMNS;
  return processBlocks(text, blockSize, encryptRouteBlock);
}

function decryptRoute(text, originalLength) {
  const blockSize = ROUTE_ROWS * ROUTE_COLUMNS;
  const result = processBlocks(text, blockSize, decryptRouteBlock);
  return trimPadding(result.output, originalLength);
}

function encryptMultiple(text) {
  const blockSize = splitToChars(NAME_KEYWORD).length * splitToChars(SURNAME_KEYWORD).length;
  return processBlocks(text, blockSize, encryptMultipleBlock);
}

function decryptMultiple(text, originalLength) {
  const blockSize = splitToChars(NAME_KEYWORD).length * splitToChars(SURNAME_KEYWORD).length;
  const result = processBlocks(text, blockSize, decryptMultipleBlock);
  return trimPadding(result.output, originalLength);
}

module.exports = {
  buildRank,
  decryptMultiple,
  decryptRoute,
  encryptMultiple,
  encryptRoute
};
