const fs = require("fs");
const path = require("path");
const { WebSocket } = require("ws");

const ws = new WebSocket("ws://localhost:3000");
let writeStream = null;
let targetPath = null;

ws.on("open", () => {
  console.log("Connected. Requesting file list...");
  ws.send(JSON.stringify({ list: true }));
});

ws.on("message", (data, isBinary) => {
  if (!isBinary) {
    const txt = data.toString();
    try {
      const obj = JSON.parse(txt);
      if (obj.files) {
        console.log("Files on server:", obj.files);
        if (obj.files.length === 0)
          return console.log("No files to download in download/");
        const pick = obj.files[0];
        console.log("Requesting file:", pick);
        ws.send(JSON.stringify({ get: pick }));
      } else if (obj.filename) {
        targetPath = path.join(
          __dirname,
          "download_received_" + Date.now() + "_" + obj.filename
        );
        writeStream = fs.createWriteStream(targetPath);
        console.log("Receiving file ->", targetPath);
      } else if (obj.done) {
        if (writeStream) writeStream.end();
        console.log("File download complete:", targetPath);
        ws.close();
      } else if (obj.error) {
        console.error("Server error:", obj.error);
      } else {
        console.log("Server message:", obj);
      }
    } catch (e) {
      console.log("Non-JSON text:", txt);
    }
  } else {
    if (writeStream) writeStream.write(data);
    else console.warn("Binary chunk received but no write stream");
  }
});
