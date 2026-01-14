const WebSocket = require("ws");

const wss = new WebSocket.Server({ port: 4000 }, () => {
  console.log("WS server listening on ws://localhost:4000");
});

wss.on("connection", (ws, req) => {
  console.log(
    "New WS client connected:",
    req.socket.remoteAddress + ":" + req.socket.remotePort
  );

  ws._state = {
    lastClientN: 0,
    kCounter: 0,
    intervalId: null,
  };

  ws._state.intervalId = setInterval(() => {
    ws._state.kCounter += 1;
    const n = ws._state.lastClientN || 0;
    const k = ws._state.kCounter;
    const msg = `10-01-server: ${n}->${k}`;
    if (ws.readyState === WebSocket.OPEN) {
      ws.send(msg);
      console.log("Sent to client:", msg);
    }
  }, 3000);

  ws.on("message", (message) => {
    const text = message.toString();
    console.log("Received from client:", text);

    const m = /10-01-client:\s*(\d+)/i.exec(text);
    if (m) {
      const n = parseInt(m[1], 10);
      ws._state.lastClientN = isNaN(n) ? ws._state.lastClientN : n;
    }
  });

  ws.on("close", () => {
    console.log("Client disconnected");
    if (ws._state.intervalId) clearInterval(ws._state.intervalId);
  });

  ws.on("error", (err) => {
    console.error("WS error:", err.message);
  });
});
