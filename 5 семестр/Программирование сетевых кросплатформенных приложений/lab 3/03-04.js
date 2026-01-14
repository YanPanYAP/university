const http = require("http");
const fs = require("fs");
const url = require("url");

const PORT = 3000;

function factSync(n) {
  if (n < 0) return null;
  if (n === 0 || n === 1) return 1;
  return n * factSync(n - 1);
}

function factNextTick(n, cb) {
  const start = process.hrtime.bigint();
  process.nextTick(() => {
    try {
      const r = factSync(n);
      const end = process.hrtime.bigint();
      const duration = Number(end - start) / 1_000_000;
      cb(null, { result: r, time_ms: duration.toFixed(3) });
    } catch (e) {
      cb(e);
    }
  });
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

    factNextTick(k, (err, data) => {
      if (err) {
        res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("Ошибка вычисления");
        return;
      }

      const t = Date.now();
      res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
      res.end(`${t}-${k}/${data.result}`);
    });

    return;
  }

  res.writeHead(404);
  res.end("Not found");
});

server.listen(PORT, () =>
  console.log(`Server running at http://localhost:${PORT}/`)
);
