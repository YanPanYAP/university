const fs = require("fs");
const path = require("path");
const { WebSocket } = require("ws");

if (process.argv.length < 3) {
  console.log("Usage: node 11-01a.js <file-to-send>");
  process.exit(1);
}

const filePath = path.resolve(process.argv[2]);
if (!fs.existsSync(filePath)) {
  console.error("File not found:", filePath);
  process.exit(1);
}

const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  console.log("Connected. Sending...");
  ws.send(JSON.stringify({ filename: path.basename(filePath) }));

  const rs = fs.createReadStream(filePath);
  rs.on("data", (chunk) => ws.send(chunk));
  rs.on("end", () => {
    console.log("File chunks sent");
    ws.send(JSON.stringify({ done: true }));
  });
});

ws.on("message", (msg) => {
  try {
    console.log("Server:", JSON.parse(msg.toString()));
  } catch (e) {
    console.log("Server:", msg.toString());
  }
});
