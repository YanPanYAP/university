const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-07 server on ws://localhost:3000")
);

wss.on("connection", (ws) => {
  console.log("Client connected to notify server");
  ws.on("message", (m) => {
    try {
      const obj = JSON.parse(m.toString());
      if (obj.notify && ["A", "B", "C"].includes(obj.notify)) {
        console.log(
          `Notification from client: ${obj.notify} at ${new Date(
            obj.timestamp || Date.now()
          ).toLocaleString()}`
        );
        ws.send(JSON.stringify({ ok: true }));
      } else {
        ws.send(JSON.stringify({ error: "invalid notify" }));
      }
    } catch (e) {
      ws.send(JSON.stringify({ error: "invalid json" }));
    }
  });
});
