const http = require("http");
const url = require("url");
const DB = require("./db");

const PORT = 3000;
const db = new DB();

function sendJson(res, statusCode, data) {
  res.writeHead(statusCode, { "Content-Type": "application/json" });
  res.end(JSON.stringify(data));
}

function parseBody(req) {
  return new Promise((resolve, reject) => {
    let body = "";
    req.on("data", (chunk) => (body += chunk));
    req.on("end", () => {
      try {
        resolve(JSON.parse(body || "{}"));
      } catch (e) {
        reject(new Error("Некорректный формат тела запроса (ожидается JSON)"));
      }
    });
  });
}

function isValidName(name) {
  return typeof name === "string" && /^[A-Za-zА-Яа-яЁё\s-]+$/.test(name.trim());
}

function isValidDate(dateStr) {
  if (typeof dateStr !== "string") return false;
  const regex = /^\d{4}-\d{2}-\d{2}$/;
  if (!regex.test(dateStr)) return false;

  const date = new Date(dateStr);
  if (isNaN(date)) return false;

  const today = new Date();
  today.setHours(0, 0, 0, 0);
  date.setHours(0, 0, 0, 0);

  return date <= today;
}

function isValidId(id) {
  return Number.isInteger(id) && id > 0;
}

const server = http.createServer(async (req, res) => {
  const parsedUrl = url.parse(req.url, true);
  const path = parsedUrl.pathname;
  const method = req.method;

  if (path === "/api/db" && method === "GET") {
    try {
      const rows = await db.select();
      sendJson(res, 200, rows);
    } catch (err) {
      sendJson(res, 500, { error: "Ошибка при получении данных из базы" });
    }
  } else if (path === "/api/db" && method === "POST") {
    try {
      const body = await parseBody(req);
      const { name, bday } = body;

      if (!isValidName(name))
        return sendJson(res, 400, {
          error: "Некорректное имя (допустимы только буквы, пробелы и дефисы)",
        });
      if (!isValidDate(bday))
        return sendJson(res, 400, {
          error:
            "Некорректная дата рождения (формат ГГГГ-ММ-ДД и не позже текущей даты)",
        });

      const newRow = await db.insert({ name, bday });
      sendJson(res, 201, newRow);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при добавлении записи",
      });
    }
  } else if (path === "/api/db" && method === "PUT") {
    try {
      const body = await parseBody(req);
      const { id, name, bday } = body;

      if (!isValidId(id))
        return sendJson(res, 400, {
          error: "Некорректный ID (должен быть целым положительным числом)",
        });
      if (name && !isValidName(name))
        return sendJson(res, 400, {
          error: "Некорректное имя (допустимы только буквы, пробелы и дефисы)",
        });
      if (bday && !isValidDate(bday))
        return sendJson(res, 400, {
          error:
            "Некорректная дата рождения (формат ГГГГ-ММ-ДД и не позже текущей даты)",
        });

      const updated = await db.update({ id, name, bday });
      sendJson(res, 200, updated);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при обновлении записи",
      });
    }
  } else if (path === "/api/db" && method === "DELETE") {
    try {
      const id = Number(parsedUrl.query.id);
      if (!isValidId(id))
        return sendJson(res, 400, { error: "Некорректный ID для удаления" });

      const deleted = await db.delete(id);
      sendJson(res, 200, deleted);
    } catch (err) {
      sendJson(res, 400, {
        error: err.message || "Ошибка при удалении записи",
      });
    }
  } else {
    sendJson(res, 404, { error: "Маршрут не найден. Проверьте URL." });
  }
});

server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}/api/db`);
});
