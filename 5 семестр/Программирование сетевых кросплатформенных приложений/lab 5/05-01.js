const http = require("http");
const url = require("url");
const fs = require("fs");
const path = require("path");
const DB = require("./db2");
const readline = require("readline");

const PORT = 3000;
const db = new DB();

let stat = {
  start: null,
  end: null,
  requests: 0,
  commits: 0,
};

let collectingStat = false;

db.on("COMMIT", () => {
  if (collectingStat) stat.commits++;
});

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
      } catch (err) {
        reject(err);
      }
    });
  });
}

function formatDate(d) {
  if (!d) return "";
  const dt = new Date(d);
  const pad = (n) => String(n).padStart(2, "0");
  return `${dt.getFullYear()}-${pad(dt.getMonth() + 1)}-${pad(
    dt.getDate()
  )} ${pad(dt.getHours())}:${pad(dt.getMinutes())}:${pad(dt.getSeconds())}`;
}

const server = http.createServer(async (req, res) => {
  const parsedUrl = url.parse(req.url, true);
  const pathname = parsedUrl.pathname;
  const method = req.method;

  if (collectingStat) stat.requests++;

  if (pathname === "/api/db" && method === "GET") {
    try {
      const rows = await db.select();
      sendJson(res, 200, rows);
    } catch (err) {
      sendJson(res, 500, { error: err.message });
    }
  } else if (pathname.startsWith("/api/db/") && method === "GET") {
    try {
      const id = Number(pathname.split("/").pop());
      if (isNaN(id)) return sendJson(res, 400, { error: "Некорректный ID" });

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
      const inserted = await db.insert(body);
      sendJson(res, 201, inserted);
    } catch (err) {
      sendJson(res, 400, { error: err.message });
    }
  } else if (pathname === "/api/db" && method === "PUT") {
    try {
      const body = await parseBody(req);
      const updated = await db.update(body);
      sendJson(res, 200, updated);
    } catch (err) {
      sendJson(res, 400, { error: err.message });
    }
  } else if (pathname === "/api/db" && method === "DELETE") {
    try {
      const id = Number(parsedUrl.query.id);
      if (isNaN(id)) return sendJson(res, 400, { error: "id must be number" });
      const deleted = await db.delete(id);
      sendJson(res, 200, deleted);
    } catch (err) {
      sendJson(res, 400, { error: err.message });
    }
  } else if (pathname === "/api/ss") {
    const formattedStat = {
      start: formatDate(stat.start),
      end: formatDate(stat.end),
      requests: stat.requests,
      commits: stat.commits,
    };
    sendJson(res, 200, formattedStat);
  } else if (pathname === "/") {
    const filePath = path.join(__dirname, "index.html");
    fs.readFile(filePath, (err, data) => {
      if (err) {
        res.writeHead(500, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("Ошибка при загрузке index.html");
      } else {
        res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
        res.end(data);
      }
    });
  } else {
    sendJson(res, 404, { error: "Не найдено" });
  }
});

server.listen(PORT, () => {
  console.log(`Server started at http://localhost:${PORT}`);
});

let stopTimer = null;
let commitTimer = null;
let statTimer = null;

db.commit = function () {
  this.emit("COMMIT");
  console.log(`[${new Date().toLocaleTimeString()}] DB committed`);
};

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

rl.on("line", (line) => {
  const [cmd, arg] = line.trim().split(" ");

  switch (cmd) {
    case "sd":
      if (!arg) {
        if (stopTimer) {
          clearTimeout(stopTimer);
          stopTimer = null;
          console.log("Остановка сервера отменена");
        }
      } else {
        const sec = Number(arg);
        if (isNaN(sec)) return console.log("sd x — x должно быть числом");
        if (stopTimer) clearTimeout(stopTimer);
        stopTimer = setTimeout(() => {
          console.log("Сервер остановлен");
          server.close(() => process.exit(0));
        }, sec * 1000);
        stopTimer.unref();
        console.log(`Сервер остановится через ${sec} секунд`);
      }
      break;

    case "sc":
      if (!arg) {
        if (commitTimer) {
          clearInterval(commitTimer);
          commitTimer = null;
          console.log("Commit остановлен");
        }
      } else {
        const sec = Number(arg);
        if (isNaN(sec)) return console.log("sc x — x должно быть числом");
        if (commitTimer) clearInterval(commitTimer);
        commitTimer = setInterval(() => db.commit(), sec * 1000);
        commitTimer.unref();
        console.log(`Commit каждые ${sec} секунд`);
      }
      break;

    case "ss":
      if (!arg) {
        if (statTimer) {
          clearTimeout(statTimer);
          statTimer = null;
          stat.end = new Date().toISOString();
          collectingStat = false;
          console.log("Сбор статистики остановлен");
        } else {
          console.log("Сбор статистики не запущен");
        }
      } else {
        const sec = Number(arg);
        if (isNaN(sec)) return console.log("ss x — x должно быть числом");

        stat.start = new Date().toISOString();
        stat.end = null;
        stat.requests = 0;
        stat.commits = 0;
        collectingStat = true;

        if (statTimer) {
          clearTimeout(statTimer);
          statTimer = null;
        }

        console.log(`Сбор статистики начат на ${sec} секунд`);

        statTimer = setTimeout(() => {
          stat.end = new Date().toISOString();
          statTimer = null;
          collectingStat = false;
          console.log("Сбор статистики завершен (автоматически)");
        }, sec * 1000);
        statTimer.unref();
      }
      break;

    default:
      console.log("Команды: sd x | sc x | ss x");
  }
});
