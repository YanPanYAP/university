const http = require("http");
const url = require("url");

const PORT = 3000;

function fact(n) {
  if (n < 0) return null;
  if (n === 0 || n === 1) return 1;
  return n * fact(n - 1);
}

const server = http.createServer((req, res) => {
  const u = url.parse(req.url, true);
  if (u.pathname === "/fact") {
    const k = parseInt(u.query.k, 10);
    if (Number.isNaN(k) || k < 0) {
      res.writeHead(400, { "Content-Type": "application/json; charset=utf-8" });
      res.end(
        JSON.stringify({ error: "k должно быть неотрицательным числом" })
      );
      return;
    }
    const f = fact(k);
    res.writeHead(200, { "Content-Type": "application/json; charset=utf-8" });
    res.end(JSON.stringify({ k: k, fact: f }));
    return;
  }

  res.writeHead(404);
  res.end("Not found");
});

server.listen(PORT, () =>
  console.log(`Server running at http://localhost:3000/fact?k=4`)
);
