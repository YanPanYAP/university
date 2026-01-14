const http = require("http");

http.get(
  {
    hostname: "localhost",
    port: 3000,
    path: "/subscribe",
    headers: { Accept: "text/event-stream" },
  },
  (res) => {
    res.on("data", (chunk) => console.log(chunk.toString()));
  }
);
