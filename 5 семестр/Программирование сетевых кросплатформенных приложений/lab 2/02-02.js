const http = require("http");
const fs = require("fs");
const path = require("path");

http
  .createServer((req, res) => {
    if (req.url === "/png") {
      const filePath = path.join(__dirname, "pic.png");
      fs.readFile(filePath, (err, data) => {
        if (err) {
          res.writeHead(500);
          res.end("Ошибка чтения изображения");
        } else {
          res.writeHead(200, { "Content-Type": "image/png" });
          res.end(data);
        }
      });
    } else {
      res.writeHead(404);
      res.end("Not Found");
    }
  })
  .listen(3000, () => {
    console.log("Сервер запущен на http://localhost:3000/png");
  });
