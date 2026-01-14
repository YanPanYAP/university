const http = require("http");
const fs = require("fs");
const path = require("path");

const PORT = 3008;

http
  .createServer((req, res) => {
    if (req.method === "GET") {
      const filename = path.join(__dirname, "ServerFile.png");

      if (!fs.existsSync(filename)) {
        res.writeHead(404);
        return res.end("Файл не найден на сервере");
      }

      res.writeHead(200, {
        "Content-Type": "application/octet-stream",
        "Content-Disposition": 'attachment; filename="ServerFile.png"',
      });

      fs.createReadStream(filename).pipe(res);
    } else {
      res.writeHead(405);
      res.end("Требуется GET запрос");
    }
  })
  .listen(PORT, () => console.log(`Server 09-08 running on port ${PORT}`));
