const WebSocket = require("ws");
const readline = require("readline");

const ID = process.argv[2] || "Client";

const ws = new WebSocket("ws://localhost:4100");

ws.on("open", () => {
  console.log(`[${ID}] Connected`);
});

ws.on("message", (msg) => {
  console.log(`[${ID}] Received: ${msg}`);
});

ws.on("close", () => {
  console.log(`[${ID}] Closed`);
});

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

rl.on("line", (line) => {
  ws.send(`${ID}: ${line}`);
});
