const http = require("http");
const fs = require("fs");
const querystring = require("querystring");
const { send } = require("./m0603");

const PORT = 3000;

const server = http.createServer((req, res) => {
  if (req.method === "GET" && req.url === "/") {
    fs.readFile("index.html", (err, data) => {
      if (err) {
        res.writeHead(500);
        res.end("Ошибка сервера");
        return;
      }
      res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
      res.end(data);
    });
  } else if (req.method === "POST" && req.url === "/send") {
    let body = "";
    req.on("data", (chunk) => (body += chunk));
    req.on("end", async () => {
      const parsed = querystring.parse(body);
      const { message } = parsed;

      await send(message);

      res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
      res.end(`<p>Сообщение отправлено!</p><a href="/">Назад</a>`);
    });
  } else {
    res.writeHead(404);
    res.end("Страница не найдена");
  }
});

server.listen(PORT, () =>
  console.log(`Сервер работает на http://localhost:${PORT}`)
);
