const http = require("http");
const fs = require("fs");
const path = require("path");

const PORT = 3007;

http
  .createServer((req, res) => {
    if (req.method === "POST") {
      const filename = path.join(__dirname, "Uploaded_MyFile.png");
      const fileStream = fs.createWriteStream(filename);

      req.pipe(fileStream);

      req.on("end", () => {
        res.writeHead(200, { "Content-Type": "text/plain" });
        res.end("Файл успешно получен и сохранён.");
      });

      req.on("error", (err) => console.error("Ошибка запроса:", err));
    } else {
      res.writeHead(405);
      res.end("Требуется POST запрос");
    }
  })
  .listen(PORT, () => console.log(`Server 09-07 running on port ${PORT}`));
