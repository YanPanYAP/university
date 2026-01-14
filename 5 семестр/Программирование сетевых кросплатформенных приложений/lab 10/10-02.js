const WebSocket = require("ws");

const ws = new WebSocket("ws://localhost:4000");

let clientCounter = 0;
let sendInterval = null;
let stopTimeout = null;

ws.on("open", () => {
  console.log("Connected to ws://localhost:4000");

  sendInterval = setInterval(() => {
    clientCounter += 1;
    const msg = `10-01-client: ${clientCounter}`;
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(msg);
      console.log("Sent:", msg);
    }
  }, 3000);

  stopTimeout = setTimeout(() => {
    clearInterval(sendInterval);
    ws.close();
    console.log("25s and closed");
  }, 25000);
});

ws.on("message", (data) => {
  console.log("Received:", data.toString());
});

ws.on("close", () => {
  console.log("Connection closed");
  if (sendInterval) clearInterval(sendInterval);
  if (stopTimeout) clearTimeout(stopTimeout);
});

ws.on("error", (err) => {
  console.error("WS error:", err.message);
});
