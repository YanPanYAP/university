const http = require("http");
const fs = require("fs");

http
  .createServer((req, res) => {
    if (req.method === "POST" && req.url === "/upload") {
      const fileStream = fs.createWriteStream("Uploaded_MyFile.txt");

      req.pipe(fileStream);

      req.on("end", () => {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end("Файл успешно получен сервером.");
      });

      req.on("error", (err) => {
        console.error(err);
        res.writeHead(500);
        res.end("Ошибка загрузки файла.");
      });
    } else {
      res.writeHead(404);
      res.end("Not found");
    }
  })
  .listen(3006, () => console.log("Server running on http://localhost:3006"));
