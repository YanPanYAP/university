const { WebSocket } = require("ws");
const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  ws.send(JSON.stringify({ subscribe: "B" }));
  console.log("Subscribed to B");
});
ws.on("message", (m) => {
  try {
    const obj = JSON.parse(m.toString());
    if (obj.event === "B")
      console.log(
        "Event B happened at",
        new Date(obj.timestamp).toLocaleString()
      );
  } catch (e) {}
});
