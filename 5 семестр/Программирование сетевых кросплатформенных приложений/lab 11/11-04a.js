const { WebSocket } = require("ws");

const clientName = process.argv[2] || "client_default";
const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  const msg = { client: clientName, timestamp: Date.now() };
  console.log("Send:", msg);
  ws.send(JSON.stringify(msg));
});

ws.on("message", (m) => {
  console.log("Server reply:", m.toString());
  ws.close();
});
