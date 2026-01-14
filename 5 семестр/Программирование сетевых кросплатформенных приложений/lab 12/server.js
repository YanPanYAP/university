const http = require("http");
const fs = require("fs");
const path = require("path");

const PORT = 3000;
const DATA_FILE = path.join(__dirname, "StudentList.json");
const BACKUP_DIR = path.join(__dirname, "backups");

if (!fs.existsSync(BACKUP_DIR)) fs.mkdirSync(BACKUP_DIR);

let subscribers = [];

function readStudents() {
  try {
    return JSON.parse(fs.readFileSync(DATA_FILE));
  } catch {
    throw { error: 1, message: `ошибка чтения файла ${DATA_FILE}` };
  }
}

function writeStudents(data) {
  fs.writeFileSync(DATA_FILE, JSON.stringify(data, null, 2));
}

function send(res, code, obj) {
  res.writeHead(code, { "Content-Type": "application/json" });
  res.end(JSON.stringify(obj));
}

function notifySubscribers(msg) {
  subscribers.forEach((res) => res.write(`data: ${msg}\n\n`));
}

const server = http.createServer((req, res) => {
  const url = decodeURI(req.url);
  const method = req.method;

  if (url === "/subscribe") {
    res.writeHead(200, {
      "Content-Type": "text/event-stream",
      "Cache-Control": "no-cache",
      Connection: "keep-alive",
    });

    subscribers.push(res);
    req.on("close", () => {
      subscribers = subscribers.filter((r) => r !== res);
    });
    return;
  }

  if (method === "GET" && url === "/") {
    try {
      send(res, 200, readStudents());
    } catch (e) {
      send(res, 500, e);
    }
  } else if (method === "GET" && /^\/\d+$/.test(url)) {
    const id = Number(url.slice(1));
    try {
      const students = readStudents();
      const s = students.find((x) => x.id === id);
      if (!s)
        return send(res, 404, {
          error: 2,
          message: `студент с id равным ${id} не найден`,
        });
      send(res, 200, s);
    } catch (e) {
      send(res, 500, e);
    }
  } else if (method === "POST" && url === "/") {
    let body = "";
    req.on("data", (chunk) => (body += chunk));
    req.on("end", () => {
      const student = JSON.parse(body);
      const students = readStudents();

      if (students.find((s) => s.id === student.id))
        return send(res, 409, {
          error: 3,
          message: `студент с id равным ${student.id} уже есть`,
        });

      students.push(student);
      writeStudents(students);
      send(res, 200, student);
    });
  } else if (method === "PUT" && url === "/") {
    let body = "";
    req.on("data", (c) => (body += c));
    req.on("end", () => {
      const student = JSON.parse(body);
      const students = readStudents();
      const idx = students.findIndex((s) => s.id === student.id);

      if (idx === -1)
        return send(res, 404, {
          error: 2,
          message: `студент с id равным ${student.id} не найден`,
        });

      students[idx] = student;
      writeStudents(students);
      send(res, 200, student);
    });
  } else if (method === "DELETE" && /^\/\d+$/.test(url)) {
    const id = Number(url.slice(1));
    const students = readStudents();
    const idx = students.findIndex((s) => s.id === id);

    if (idx === -1)
      return send(res, 404, {
        error: 2,
        message: `студент с id равным ${id} не найден`,
      });

    const removed = students.splice(idx, 1)[0];
    writeStudents(students);
    send(res, 200, removed);
  } else if (method === "POST" && url === "/backup") {
    setTimeout(() => {
      const d = new Date();
      const stamp =
        d.getFullYear().toString() +
        (d.getMonth() + 1).toString().padStart(2, "0") +
        d.getDate().toString().padStart(2, "0") +
        d.getHours().toString().padStart(2, "0") +
        d.getSeconds().toString().padStart(2, "0");

      const file = `${stamp}_StudentList.json`;
      fs.copyFileSync(DATA_FILE, path.join(BACKUP_DIR, file));
      notifySubscribers(`Создан backup: ${file}`);
    }, 2000);

    send(res, 200, { message: "backup created" });
  } else if (method === "GET" && url === "/backup") {
    send(res, 200, fs.readdirSync(BACKUP_DIR));
  } else if (method === "DELETE" && /^\/backup\/\d{8}$/.test(url)) {
  const yyyyddmm = url.split("/")[2];

  const year  = Number(yyyyddmm.slice(0, 4));
  const day   = Number(yyyyddmm.slice(4, 6));
  const month = Number(yyyyddmm.slice(6, 8));

  if (month < 1 || month > 12 || day < 1 || day > 31) {
    send(res, 400, { error: "Invalid date format YYYYDDMM" });
    return;
  }

  const limitDate = new Date(year, month - 1, day);

  const files = fs.readdirSync(BACKUP_DIR);
  const removed = [];

  files.forEach(file => {
    const match = file.match(
      /^(\d{4})(\d{2})(\d{2})\d{4}_StudentList\.json$/
    );
    if (!match) return;

    const fileYear  = Number(match[1]);
    const fileMonth = Number(match[2]);
    const fileDay   = Number(match[3]);

    const fileDate = new Date(fileYear, fileMonth - 1, fileDay);

    if (fileDate < limitDate) {
      fs.unlinkSync(path.join(BACKUP_DIR, file));
      removed.push(file);
    }
  });

  send(res, 200, {
    message: `Removed backups older than ${yyyyddmm}`,
    removed
  });
}
 else {
    res.writeHead(404);
    res.end();
  }
});

fs.watch(BACKUP_DIR, () => {
  notifySubscribers("Изменена копия StudentList.json");
});

server.listen(PORT, () =>
  console.log(`Server running at http://localhost:${PORT}`)
);
