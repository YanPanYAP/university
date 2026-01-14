const http = require("http");
const url = require("url");
const fs = require("fs");
const path = require("path");
const DB = require("./db2");

const PORT = 3000;
const db = new DB();

function sendJson(res, statusCode, data) {
  res.writeHead(statusCode, {
    "Content-Type": "application/json; charset=utf-8",
  });
  res.end(JSON.stringify(data));
}

function parseBody(req) {
  return new Promise((resolve, reject) => {
    let body = "";
    req.on("data", (chunk) => (body += chunk));
    req.on("end", () => {
      try {
        resolve(JSON.parse(body || "{}"));
      } catch (err) {
        reject(new Error("Ошибка: тело запроса должно быть корректным JSON"));
      }
    });
  });
}

const server = http.createServer(async (req, res) => {
  const parsedUrl = url.parse(req.url, true);
  const pathname = parsedUrl.pathname;
  const method = req.method;

  if (pathname === "/api/db" && method === "GET") {
    try {
      const rows = await db.select();
      sendJson(res, 200, rows);
    } catch (err) {
      sendJson(res, 500, { error: "Ошибка при получении данных из базы" });
    }
  } else if (pathname.startsWith("/api/db/") && method === "GET") {
    try {
      const id = Number(pathname.split("/").pop());
      if (isNaN(id))
        return sendJson(res, 400, {
          error: "Некорректный ID (ожидается число)",
        });

      const rows = await db.select();
      const found = rows.find((r) => r.id === id);
      if (!found)
        return sendJson(res, 404, { error: `Запись с ID ${id} не найдена` });

      sendJson(res, 200, found);
    } catch (err) {
      sendJson(res, 400, { error: "Ошибка при поиске записи по ID" });
    }
  } else if (pathname === "/api/db" && method === "POST") {
    try {
      const body = await parseBody(req);
      if (!body || Object.keys(body).length === 0)
        return sendJson(res, 400, { error: "Пустое тело запроса" });

      const inserted = await db.insert(body);
      sendJson(res, 201, inserted);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при добавлении записи",
      });
    }
  } else if (pathname === "/api/db" && method === "PUT") {
    try {
      const body = await parseBody(req);
      if (!body.id)
        return sendJson(res, 400, { error: "Не указан ID для обновления" });

      const updated = await db.update(body);
      sendJson(res, 200, updated);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при обновлении записи",
      });
    }
  } else if (pathname === "/api/db" && method === "DELETE") {
    try {
      const id = Number(parsedUrl.query.id);
      if (isNaN(id))
        return sendJson(res, 400, { error: "Некорректный ID для удаления" });

      const deleted = await db.delete(id);
      sendJson(res, 200, deleted);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при удалении записи",
      });
    }
  } else if (pathname === "/") {
    const filePath = path.join(__dirname, "index.html");
    fs.readFile(filePath, (err, data) => {
      if (err) {
        res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(
          "Ошибка при загрузке index.html. Проверьте, существует ли файл."
        );
      } else {
        res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
        res.end(data);
      }
    });
  } else {
    sendJson(res, 404, { error: `Маршрут ${pathname} не найден` });
  }
});

server.listen(PORT, () => {
  console.log(`Сервер запущен: http://localhost:${PORT}`);
});
