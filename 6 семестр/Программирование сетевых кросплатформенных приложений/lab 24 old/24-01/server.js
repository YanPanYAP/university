require("dotenv").config();

const express = require("express");
const multer = require("multer");
const path = require("path");
const { createClient } = require("webdav");

const app = express();
const upload = multer({ storage: multer.memoryStorage() });

const PORT = Number(process.env.PORT || 3000);
const WEBDAV_URL = process.env.WEBDAV_URL;
const WEBDAV_USERNAME = process.env.WEBDAV_USERNAME;
const WEBDAV_PASSWORD = process.env.WEBDAV_PASSWORD;
const WEBDAV_TOKEN = process.env.WEBDAV_TOKEN;
const WEBDAV_BASE_PATH = (process.env.WEBDAV_BASE_PATH || "/").trim();

if (!WEBDAV_URL) {
  throw new Error("Environment variable WEBDAV_URL is required");
}

if (!WEBDAV_TOKEN && (!WEBDAV_USERNAME || !WEBDAV_PASSWORD)) {
  throw new Error(
    "Set WEBDAV_TOKEN or both WEBDAV_USERNAME and WEBDAV_PASSWORD"
  );
}

const clientOptions = WEBDAV_TOKEN
  ? { headers: { Authorization: `Bearer ${WEBDAV_TOKEN}` } }
  : { username: WEBDAV_USERNAME, password: WEBDAV_PASSWORD };

const webdav = createClient(WEBDAV_URL, clientOptions);

function toStoragePath(name) {
  const base = WEBDAV_BASE_PATH === "/" ? "" : WEBDAV_BASE_PATH.replace(/\/+$/, "");
  return path.posix.join(base || "/", name);
}

function normalizeName(raw) {
  const decoded = decodeURIComponent(String(raw || "")).trim();
  if (!decoded || decoded.includes("/") || decoded.includes("\\")) {
    return null;
  }
  return decoded;
}

async function existsAsType(remotePath, type) {
  if (!(await webdav.exists(remotePath))) {
    return false;
  }

  const stat = await webdav.stat(remotePath);
  return stat.type === type;
}

function handleUnexpectedError(res, error, fallbackMessage, fallbackStatus = 408) {
  const status = error?.status === 404 ? 404 : fallbackStatus;
  res.status(status).json({ error: fallbackMessage, details: error.message });
}

app.post("/md/:name", async (req, res) => {
  const dirName = normalizeName(req.params.name);
  if (!dirName) {
    return res.status(400).json({ error: "Invalid directory name" });
  }

  const remotePath = toStoragePath(dirName);

  try {
    if (await webdav.exists(remotePath)) {
      return res.status(408).json({ error: "Directory already exists" });
    }

    await webdav.createDirectory(remotePath);
    return res.status(200).json({ message: `Directory '${dirName}' created` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot create directory", 408);
  }
});

app.post("/rd/:name", async (req, res) => {
  const dirName = normalizeName(req.params.name);
  if (!dirName) {
    return res.status(400).json({ error: "Invalid directory name" });
  }

  const remotePath = toStoragePath(dirName);

  try {
    if (!(await existsAsType(remotePath, "directory"))) {
      return res.status(408).json({ error: "Directory does not exist" });
    }

    await webdav.deleteFile(remotePath);
    return res.status(200).json({ message: `Directory '${dirName}' deleted` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot delete directory", 408);
  }
});

app.post("/up/:name", upload.single("file"), async (req, res) => {
  const fileName = normalizeName(req.params.name);
  if (!fileName) {
    return res.status(400).json({ error: "Invalid file name" });
  }

  if (!req.file || !req.file.buffer) {
    return res
      .status(400)
      .json({ error: "Attach file as multipart/form-data field 'file'" });
  }

  const remotePath = toStoragePath(fileName);

  try {
    await webdav.putFileContents(remotePath, req.file.buffer, { overwrite: true });
    return res.status(200).json({ message: `File '${fileName}' uploaded` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot write file", 408);
  }
});

app.post("/down/:name", async (req, res) => {
  const fileName = normalizeName(req.params.name);
  if (!fileName) {
    return res.status(400).json({ error: "Invalid file name" });
  }

  const remotePath = toStoragePath(fileName);

  try {
    if (!(await existsAsType(remotePath, "file"))) {
      return res.status(404).json({ error: "File not found" });
    }

    const fileBuffer = await webdav.getFileContents(remotePath, { format: "binary" });
    res.setHeader("Content-Disposition", `attachment; filename="${fileName}"`);
    res.setHeader("Content-Type", "application/octet-stream");
    return res.status(200).send(fileBuffer);
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot download file", 408);
  }
});

app.post("/del/:name", async (req, res) => {
  const fileName = normalizeName(req.params.name);
  if (!fileName) {
    return res.status(400).json({ error: "Invalid file name" });
  }

  const remotePath = toStoragePath(fileName);

  try {
    if (!(await existsAsType(remotePath, "file"))) {
      return res.status(404).json({ error: "File not found" });
    }

    await webdav.deleteFile(remotePath);
    return res.status(200).json({ message: `File '${fileName}' deleted` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot delete file", 408);
  }
});

app.post("/copy/:source/:target", async (req, res) => {
  const sourceName = normalizeName(req.params.source);
  const targetName = normalizeName(req.params.target);

  if (!sourceName || !targetName) {
    return res.status(400).json({ error: "Invalid file name" });
  }

  const sourcePath = toStoragePath(sourceName);
  const targetPath = toStoragePath(targetName);

  try {
    if (!(await existsAsType(sourcePath, "file"))) {
      return res.status(404).json({ error: "Source file not found" });
    }

    await webdav.copyFile(sourcePath, targetPath, { overwrite: true });
    return res
      .status(200)
      .json({ message: `File '${sourceName}' copied to '${targetName}'` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot copy file", 408);
  }
});

app.post("/move/:source/:target", async (req, res) => {
  const sourceName = normalizeName(req.params.source);
  const targetName = normalizeName(req.params.target);

  if (!sourceName || !targetName) {
    return res.status(400).json({ error: "Invalid file name" });
  }

  const sourcePath = toStoragePath(sourceName);
  const targetPath = toStoragePath(targetName);

  try {
    if (!(await existsAsType(sourcePath, "file"))) {
      return res.status(404).json({ error: "Source file not found" });
    }

    await webdav.moveFile(sourcePath, targetPath, { overwrite: true });
    return res
      .status(200)
      .json({ message: `File '${sourceName}' moved to '${targetName}'` });
  } catch (error) {
    return handleUnexpectedError(res, error, "Cannot move file", 408);
  }
});

app.use((req, res) => {
  res.status(404).json({ error: "Route not found" });
});

app.listen(PORT, () => {
  console.log(`24-01 server started on port ${PORT}`);
});
