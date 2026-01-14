const http = require("http");
const fs = require("fs");
const url = require("url");

const PORT = 3000;

function fact(n) {
  if (n < 0) return null;
  if (n === 0 || n === 1) return 1;
  return n * fact(n - 1);
}

const clientHtml = fs.readFileSync("./client.html", "utf8");

const server = http.createServer((req, res) => {
  const u = url.parse(req.url, true);

  if (u.pathname === "/") {
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
    res.end(clientHtml);
    return;
  }

  if (u.pathname === "/fact") {
    const k = parseInt(u.query.k, 10);
    if (Number.isNaN(k) || k < 0) {
      res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
      res.end("Ошибка: k должно быть неотрицательным числом");
      return;
    }
    const f = fact(k);
    const t = Date.now();

    res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
    res.end(`${t}-${k}/${f}`);
    return;
  }

  res.writeHead(404);
  res.end("Not found");
});

server.listen(PORT, () =>
  console.log(`Server running at http://localhost:${PORT}/`)
);
