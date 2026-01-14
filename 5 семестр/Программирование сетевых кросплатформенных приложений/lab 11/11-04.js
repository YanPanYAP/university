const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-04 server listening on ws://localhost:3000")
);
let n = 1;

wss.on("connection", (ws) => {
  ws.on("message", (data) => {
    try {
      const obj = JSON.parse(data.toString());
      const clientName = obj.client || "unknown";
      const timestamp = obj.timestamp || Date.now();
      const reply = { server: n++, client: clientName, timestamp };
      ws.send(JSON.stringify(reply));
      console.log("Handled from", clientName, "->", reply);
    } catch (e) {
      ws.send(JSON.stringify({ error: "invalid json" }));
    }
  });
});
