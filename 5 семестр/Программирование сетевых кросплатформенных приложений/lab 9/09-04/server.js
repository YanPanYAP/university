const http = require("http");
const url = require("url");

const server = http.createServer((req, res) => {
  const parsed = url.parse(req.url, true);
  const pathname = parsed.pathname;

  if (pathname === "/json" && req.method === "POST") {
    let body = "";

    req.on("data", (chunk) => (body += chunk));
    req.on("end", () => {
      try {
        let data = JSON.parse(body);

        let x = parseFloat(data.x);
        let y = parseFloat(data.y);
        let s = data.s || "";
        let m = Array.isArray(data.m) ? data.m : [];
        let o = data.o || {};

        let response = {
          _comment: "ответ",
          x_plus_y: x + y,
          Concatenation_s_o: s + ": " + Object.values(o).join(", "),
          Length_m: m.length,
        };

        res.writeHead(200, {
          "Content-Type": "application/json; charset=utf-8",
        });
        res.end(JSON.stringify(response, null, 2));
      } catch (err) {
        res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("Ошибка: некорректный JSON");
      }
    });
    return;
  }

  res.writeHead(404);
  res.end("Not found");
});

server.listen(3004, () =>
  console.log("Server running on http://localhost:3004/json")
);
