const http = require("http");
const fs = require("fs");
const path = require("path");

const options = {
  hostname: "localhost",
  port: 3008,
  path: "/",
  method: "GET",
};

const req = http.request(options, (res) => {
  if (res.statusCode !== 200) {
    return console.log("Ошибка:", res.statusCode);
  }

  const file = fs.createWriteStream(path.join(__dirname, "ReceivedFile.png"));
  res.pipe(file);

  file.on("finish", () => {
    file.close();
    console.log("Файл получен и сохранён.");
  });
});

req.end();
