const { WebSocket } = require("ws");
const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  ws.send(JSON.stringify({ subscribe: "A" }));
  console.log("Subscribed to A");
});
ws.on("message", (m) => {
  try {
    const obj = JSON.parse(m.toString());
    if (obj.event === "A")
      console.log(
        "Event A happened at",
        new Date(obj.timestamp).toLocaleString()
      );
  } catch (e) {}
});
