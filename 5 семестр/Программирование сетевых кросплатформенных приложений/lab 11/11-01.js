const fs = require("fs");
const path = require("path");
const { WebSocketServer } = require("ws");

const UPLOAD_DIR = path.join(__dirname, "upload");
if (!fs.existsSync(UPLOAD_DIR)) fs.mkdirSync(UPLOAD_DIR);

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-01 server listening on ws://localhost:3000")
);

wss.on("connection", (ws) => {
  console.log("Client connected for upload");
  let writeStream = null;
  let expectedName = null;
  ws.on("message", (data, isBinary) => {
    try {
      if (!isBinary) {
        const txt = data.toString();
        const meta = JSON.parse(txt);
        if (meta.filename) {
          expectedName = path.basename(meta.filename);
          const outPath = path.join(
            UPLOAD_DIR,
            `${Date.now()}_${expectedName}`
          );
          writeStream = fs.createWriteStream(outPath);
          console.log("Receiving file ->", outPath);
          ws.send(JSON.stringify({ status: "ready", savedAs: outPath }));
        } else if (meta.done) {
          if (writeStream) writeStream.end();
          ws.send(JSON.stringify({ status: "saved" }));
          console.log("File saved");
        } else {
          ws.send(JSON.stringify({ error: "unknown meta" }));
        }
      } else {
        if (writeStream) writeStream.write(data);
        else
          ws.send(
            JSON.stringify({ error: "no write stream. send metadata first." })
          );
      }
    } catch (e) {
      ws.send(JSON.stringify({ error: e.message }));
    }
  });

  ws.on("close", () => console.log("Uploader disconnected"));
});
