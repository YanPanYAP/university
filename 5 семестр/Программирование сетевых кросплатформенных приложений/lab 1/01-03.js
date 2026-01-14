const http = require("http");
const { URL } = require("url");

const server = http.createServer((req, res) => {
  const parsedUrl = new URL(req.url, `http://${req.headers.host}`);

  let body = "";
  req.on("data", (chunk) => {
    body += chunk.toString();
  });

  req.on("end", () => {
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
    res.end(`
      <html>
        <head><title>Информация о запросе</title></head>
        <body>
          <h1>Информация о запросе</h1>
          <p><b>Метод:</b> ${req.method}</p>
          <p><b>URI:</b> ${parsedUrl.pathname}</p>
          <p><b>Параметры:</b> ${parsedUrl.searchParams.toString() || "нет"}</p>
          <p><b>Заголовки:</b> <pre>${JSON.stringify(
            req.headers,
            null,
            2
          )}</pre></p>
          <p><b>Тело:</b> ${body || "нет данных"}</p>
        </body>
      </html>
    `);
  });
});

server.listen(3000, () => {
  console.log("Сервер 01-03 запущен на http://localhost:3000");
});
