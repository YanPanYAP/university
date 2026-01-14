const http = require("http");
const fs = require("fs");
const path = require("path");

const options = {
  hostname: "localhost",
  port: 3007,
  path: "/",
  method: "POST",
  headers: {
    "Content-Type": "application/octet-stream",
  },
};

const req = http.request(options, (res) => {
  res.setEncoding("utf8");
  res.on("data", (chunk) => console.log("Ответ сервера:", chunk));
});

const file = fs.createReadStream(path.join(__dirname, "MyFile.png"));
file.pipe(req);

file.on("end", () => req.end());
file.on("error", (err) => console.error("Ошибка чтения файла:", err));
