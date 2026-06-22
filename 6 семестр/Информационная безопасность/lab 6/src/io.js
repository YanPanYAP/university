const fs = require("fs");
const path = require("path");

function ensureDir(dirPath) {
  fs.mkdirSync(dirPath, { recursive: true });
}

function writeUtf8(filePath, content) {
  ensureDir(path.dirname(filePath));
  fs.writeFileSync(filePath, content, "utf8");
}

function writeJson(filePath, value) {
  writeUtf8(filePath, JSON.stringify(value, null, 2));
}

module.exports = {
  ensureDir,
  writeJson,
  writeUtf8
};
