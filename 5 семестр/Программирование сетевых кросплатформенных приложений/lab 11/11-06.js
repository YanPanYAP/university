const readline = require("readline");
const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-06 server on ws://localhost:3000")
);

wss.on("connection", (ws) => {
  console.log("Client subscribed to events");
  ws.on("message", (m) => {
    try {
      const obj = JSON.parse(m.toString());
      if (obj.subscribe) {
        ws.sub = obj.subscribe;
        ws.send(JSON.stringify({ subscribed: ws.sub }));
      }
    } catch (e) {}
  });
});

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});
console.log("Type A, B or C and press Enter to generate event");

rl.on("line", (line) => {
  const ch = line.trim().toUpperCase();
  if (ch === "A" || ch === "B" || ch === "C") {
    const evt = { event: ch, timestamp: Date.now() };
    console.log("Generated event", evt);
    wss.clients.forEach((ws) => {
      if (ws.readyState === 1) ws.send(JSON.stringify(evt));
    });
  } else {
    console.log("Unknown input. Use A, B or C");
  }
});
