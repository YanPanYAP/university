const readline = require("readline");
const { WebSocket } = require("ws");

const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  console.log(
    "Connected to 11-07 server. Type A/B/C and press Enter to notify server."
  );
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });
  rl.on("line", (line) => {
    const ch = line.trim().toUpperCase();
    if (["A", "B", "C"].includes(ch)) {
      ws.send(JSON.stringify({ notify: ch, timestamp: Date.now() }));
    } else {
      console.log("Use A, B or C");
    }
  });
});

ws.on("message", (m) => {
  try {
    const obj = JSON.parse(m.toString());
    console.log("Server replied:", obj);
  } catch (e) {}
});
