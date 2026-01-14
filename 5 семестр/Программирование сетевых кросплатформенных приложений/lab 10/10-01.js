const http = require("http");
const fs = require("fs");
const path = require("path");

const htmlPath = path.join(__dirname, "10-01-client.html");

const server = http.createServer((req, res) => {
  if (req.method === "GET" && req.url === "/start") {
    fs.readFile(htmlPath, "utf8", (err, data) => {
      if (err) {
        res.writeHead(500);
        res.end("Server error");
        return;
      }
      res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
      res.end(data);
    });
    return;
  }

  res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
  res.end("Bad request");
});

server.listen(3000, () => {
  console.log("HTTP server running at http://localhost:3000 (use /start)");
});
