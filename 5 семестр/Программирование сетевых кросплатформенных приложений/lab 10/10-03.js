const WebSocket = require("ws");

const PORT = 4100;
const wss = new WebSocket.Server({ port: PORT }, () => {
  console.log("Broadcast on ws://localhost:" + PORT);
});

wss.on("connection", (ws, req) => {
  console.log("Client connected for broadcast");

  ws.on("message", (message) => {
    const text = message.toString();
    console.log("Received:", text);

    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send("[broadcast] " + text);
      }
    });
  });

  ws.on("close", () => {
    console.log("Client disconnected (broadcast)");
  });
});
