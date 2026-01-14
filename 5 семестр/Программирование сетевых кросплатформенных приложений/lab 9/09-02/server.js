const http = require("http");
const url = require("url");

const srv = http.createServer((req, res) => {
  const q = url.parse(req.url, true).query;
  const x = Number(q.x);
  const y = Number(q.y);
  const sum =
    Number.isFinite(x) && Number.isFinite(y) ? x + y : "invalid params";
  const body = JSON.stringify({ x, y, sum });
  res.writeHead(200, { "Content-Type": "application/json" });
  res.end(body);
});

srv.listen(3002, () =>
  console.log("09-02 server listening on http://localhost:3002")
);
