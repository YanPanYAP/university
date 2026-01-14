const http = require("http");
const os = require("os");

const srv = http.createServer((req, res) => {
  const body = "Hello from 09-01";
  res.writeHead(200, { "Content-Type": "text/plain" });
  res.end(body);
});

srv.listen(3001, () =>
  console.log("09-01 server listening on http://localhost:3001")
);
