const http = require("http");

function renderHeaders(req) {
  let headersHtml = "";
  for (const key in req.headers) {
    headersHtml += `<h3>${key}: ${req.headers[key]}</h3>`;
  }
  return headersHtml;
}

const server = http.createServer((req, res) => {
  let body = "";

  req.on("data", (chunk) => {
    body += chunk;
    console.log("Получено:", chunk.toString());
  });

  req.on("end", () => {
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });

    res.end(`<!DOCTYPE html>
<html>
<head><title>Структура запроса</title></head>
<body>
  <h1>Структура запроса</h1>
  <h2>Метод: ${req.method}</h2>
  <h2>URL: ${req.url}</h2>
  <h2>Версия HTTP: ${req.httpVersion}</h2>
  <h2>Заголовки</h2>
  ${renderHeaders(req)}
  <h2>Тело: ${body || "нет данных"}</h2>
</body>
</html>`);
  });
});

server.listen(3000, () => {
  console.log("Сервер запущен на http://localhost:3000");
});
