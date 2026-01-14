const http = require("http");
const staticHandler = require("./m07-01")("static");

const server = http.createServer((req, res) => {
  staticHandler(req, res);
});

server.listen(3000, () => {
  console.log("Server 07-01 running on http://localhost:3000");
});
