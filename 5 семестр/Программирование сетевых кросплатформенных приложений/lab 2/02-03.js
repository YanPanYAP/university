const http = require("http");

http
  .createServer((req, res) => {
    if (req.url === "/api/name" && req.method === "GET") {
      res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
      res.end("Павлович Ян Андреевич");
    } else {
      res.writeHead(404);
      res.end("Not Found");
    }
  })
  .listen(3000, () => {
    console.log("Сервер запущен на http://localhost:3000/api/name");
  });
