const { WebSocket } = require("ws");
const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => {
  ws.send(JSON.stringify({ subscribe: "C" }));
  console.log("Subscribed to C");
});
ws.on("message", (m) => {
  try {
    const obj = JSON.parse(m.toString());
    if (obj.event === "C")
      console.log(
        "Event C happened at",
        new Date(obj.timestamp).toLocaleString()
      );
  } catch (e) {}
});
