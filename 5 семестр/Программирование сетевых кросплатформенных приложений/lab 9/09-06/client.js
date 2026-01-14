const http = require("http");
const fs = require("fs");

const options = {
  hostname: "localhost",
  port: 3006,
  path: "/upload",
  method: "POST",
  headers: {
    "Content-Type": "application/octet-stream",
  },
};

const req = http.request(options, (res) => {
  console.log("Статус:", res.statusCode);

  res.on("data", (chunk) => {
    console.log("Ответ:", chunk.toString());
  });
});

req.on("error", (err) => console.error(err));

fs.createReadStream("MyFile.txt").pipe(req);
