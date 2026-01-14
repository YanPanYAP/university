const http = require("http");
const fs = require("fs");
const path = require("path");

http
  .createServer((req, res) => {
    if (req.url === "/xmlhttprequest") {
      const filePath = path.join(__dirname, "xmlhttprequest.html");
      fs.readFile(filePath, (err, data) => {
        res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
        res.end(data);
      });
    } else if (req.url === "/api/name") {
      res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
      res.end("Павлович Ян Андреевич");
    } else {
      res.writeHead(404);
      res.end("Not Found");
    }
  })
  .listen(3000, () => {
    console.log("Сервер запущен на http://localhost:3000/xmlhttprequest");
  });
