const fs = require("fs");
const path = require("path");
const { WebSocketServer } = require("ws");

const DOWNLOAD_DIR = path.join(__dirname, "download");
if (!fs.existsSync(DOWNLOAD_DIR)) fs.mkdirSync(DOWNLOAD_DIR);

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-02 server listening on ws://localhost:3000")
);

wss.on("connection", (ws) => {
  console.log("Client connected for download");

  ws.on("message", (data) => {
    try {
      const req = JSON.parse(data.toString());
      if (req.list) {
        const files = fs.readdirSync(DOWNLOAD_DIR);
        ws.send(JSON.stringify({ files }));
      } else if (req.get) {
        const fname = path.basename(req.get);
        const p = path.join(DOWNLOAD_DIR, fname);
        if (!fs.existsSync(p)) {
          ws.send(JSON.stringify({ error: "not found" }));
          return;
        }
        ws.send(JSON.stringify({ filename: fname }));
        const rs = fs.createReadStream(p);
        rs.on("data", (chunk) => ws.send(chunk));
        rs.on("end", () => ws.send(JSON.stringify({ done: true })));
      } else {
        ws.send(JSON.stringify({ error: "unknown request" }));
      }
    } catch (e) {
      ws.send(JSON.stringify({ error: e.message }));
    }
  });

  ws.on("close", () => console.log("Downloader disconnected"));
});
