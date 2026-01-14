const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-03 server listening on ws://localhost:3000")
);

let counter = 1;

wss.on("connection", (ws) => {
  ws.isAlive = true;
  ws.on("pong", () => {
    ws.isAlive = true;
  });
  console.log("Client connected to 11-03");
});

const broadcastMessage = () => {
  const msg = `11-03-server: ${counter++}`;
  wss.clients.forEach((c) => {
    if (c.readyState === 1) c.send(msg);
  });
  console.log("Broadcasted:", msg);
};

setInterval(broadcastMessage, 15_000);

setInterval(() => {
  let aliveCount = 0;
  wss.clients.forEach((ws) => {
    if (!ws.isAlive) {
      try {
        ws.terminate();
      } catch (e) {}
    } else {
      ws.isAlive = false;
      ws.ping();
      aliveCount++;
    }
  });
  console.log("Alive connections:", aliveCount);
}, 5_000);
