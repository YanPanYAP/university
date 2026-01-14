const { WebSocket } = require("ws");

const ws = new WebSocket("ws://localhost:3000");

ws.on("open", () => console.log("11-03a connected"));
ws.on("message", (m) => console.log("Received:", m.toString()));
ws.on("ping", () => ws.pong());
ws.on("close", () => console.log("Disconnected"));
