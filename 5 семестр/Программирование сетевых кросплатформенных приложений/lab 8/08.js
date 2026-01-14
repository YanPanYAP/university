const http = require("http");
const url = require("url");
const path = require("path");
const fs = require("fs");
const xml2js = require("xml2js");
const builder = require("xmlbuilder");
const Busboy = require("busboy");
const mime = require("mime-types");

const STATIC_DIR = path.join(__dirname, "static");

const server = http
  .createServer((req, res) => {
    const parsedUrl = url.parse(req.url, true);
    const pathname = parsedUrl.pathname;
    const query = parsedUrl.query;

    if (req.method === "GET") {
      if (pathname === "/connection") {
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(`KeepAliveTimeout = ${server.keepAliveTimeout}`);
        return;
      }
      if (pathname === "/connection/set") {
        let newTimeout = parseInt(query.set);
        if (!isNaN(newTimeout)) {
          server.keepAliveTimeout = newTimeout;
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(
            `Новое значение KeepAliveTimeout = ${server.keepAliveTimeout}`
          );
        } else {
          res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
          res.end("Ошибка: параметр set должен быть числом");
        }
        return;
      }

      if (pathname === "/headers") {
        res.setHeader("X-Custom-Header", "MyCustomHeaderValue");
        let output =
          "Request headers:\n" + JSON.stringify(req.headers, null, 2) + "\n\n";
        output +=
          "Response headers:\n" + JSON.stringify(res.getHeaders(), null, 2);
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(output);
        return;
      }

      if (
        pathname === "/parameter" &&
        query.x !== undefined &&
        query.y !== undefined
      ) {
        let x = parseFloat(query.x);
        let y = parseFloat(query.y);
        if (!isNaN(x) && !isNaN(y)) {
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(
            `x+y=${x + y}, x-y=${x - y}, x*y=${x * y}, x/y=${
              y !== 0 ? x / y : "NaN"
            }`
          );
        } else {
          res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
          res.end("Ошибка: x и y должны быть числами");
        }
        return;
      }

      if (/^\/parameter\/\d+\/\d+$/.test(pathname)) {
        let parts = pathname.split("/");
        let x = parseFloat(parts[2]);
        let y = parseFloat(parts[3]);
        if (!isNaN(x) && !isNaN(y)) {
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(
            `x+y=${x + y}, x-y=${x - y}, x*y=${x * y}, x/y=${
              y !== 0 ? x / y : "NaN"
            }`
          );
        } else {
          res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(`URI: ${pathname}`);
        }
        return;
      }

      if (pathname === "/close") {
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end("Сервер закроется через 10 секунд");
        setTimeout(
          () => server.close(() => console.log("Server stopped")),
          10000
        );
        return;
      }

      if (pathname === "/socket") {
        res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
        res.end(
          `Client: ${req.socket.remoteAddress}:${req.socket.remotePort}\nServer: ${req.socket.localAddress}:${req.socket.localPort}`
        );
        return;
      }

      if (pathname === "/req-data") {
        let body = "";
        let parts = 0;
        req.on("data", (chunk) => {
          body += chunk;
          parts++;
        });
        req.on("end", () => {
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(
            `Длина сообщения: ${body.length}\nКоличество порций: ${parts}`
          );
        });
        return;
      }

      if (pathname === "/resp-status") {
        let code = parseInt(query.code);
        let mess = query.mess || "no message";
        if (!isNaN(code)) {
          res.writeHead(code, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(`Статус: ${code} - ${mess}`);
        } else {
          res.writeHead(400, { "Content-Type": "text/plain; charset=utf-8" });
          res.end("Ошибка: code должен быть числом");
        }
        return;
      }

      if (pathname === "/formparameter") {
        fs.readFile("./form.html", "utf8", (err, data) => {
          if (err) {
            res.writeHead(500);
            return res.end(err.message);
          }
          res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
          res.end(data);
        });
        return;
      }

      if (pathname === "/files") {
        fs.readdir(STATIC_DIR, (err, files) => {
          if (err) {
            res.writeHead(500);
            return res.end(err.message);
          }
          res.writeHead(200, {
            "Content-Type": "text/plain; charset=utf-8",
            "X-static-files-count": files.length,
          });
          res.end(`Файлов в static: ${files.length}`);
        });
        return;
      }

      if (pathname.startsWith("/files/")) {
        let filename = pathname.slice(7);
        let filepath = path.join(STATIC_DIR, filename);
        let mimeType = mime.lookup(filename) || "application/octet-stream";
        fs.readFile(filepath, (err, data) => {
          if (err) {
            res.writeHead(404);
            return res.end("Файл не найден");
          }
          res.writeHead(200, { "Content-Type": mimeType });
          res.end(data);
        });
        return;
      }

      if (pathname === "/upload") {
        res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
        res.end(`
                <form method="POST" action="/upload" enctype="multipart/form-data">
                    <input type="file" name="file"><br>
                    <input type="submit" value="Загрузить">
                </form>
            `);
        return;
      }
    }

    if (req.method === "POST") {
      if (pathname === "/formparameter") {
        let body = "";
        req.on("data", (chunk) => (body += chunk));
        req.on("end", () => {
          let params = new URLSearchParams(body);
          let output = "";
          for (let [k, v] of params.entries()) output += `${k}: ${v}\n`;
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end(output);
        });
        return;
      }

      if (pathname === "/json") {
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
              _comment: "Ответ. Лабораторная 8/10",
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

      if (pathname === "/xml") {
        let body = "";
        req.on("data", (chunk) => (body += chunk));
        req.on("end", () => {
          xml2js.parseString(body, { explicitArray: false }, (err, result) => {
            if (err || !result?.request) {
              res.writeHead(400, {
                "Content-Type": "text/plain; charset=utf-8",
              });
              return res.end("Ошибка: некорректный XML");
            }
            let request = result.request;
            let requestId = request.$?.id || "unknown";

            let xElems = Array.isArray(request.x) ? request.x : [request.x];
            let xValues = xElems.map((el) => parseFloat(el.$?.value) || 0);
            let mElems = Array.isArray(request.m) ? request.m : [request.m];
            let mValues = mElems.map((el) => el.$?.value || "");

            let numericSum = xValues.reduce((a, b) => a + b, 0);
            let concat = mValues.join("");

            let xmlResponse = builder
              .create("response")
              .att("id", requestId)
              .ele("sum")
              .att("element", "x")
              .att("result", numericSum.toString())
              .up()
              .ele("concat")
              .att("element", "m")
              .att("result", concat)
              .end({ pretty: true });

            res.writeHead(200, {
              "Content-Type": "application/xml; charset=utf-8",
            });
            res.end(xmlResponse);
          });
        });
        return;
      }

      if (pathname === "/upload") {
        let busboy = Busboy({ headers: req.headers });
        busboy.on("file", (fieldname, file, info) => {
          let { filename } = info;
          let savePath = path.join(STATIC_DIR, filename);
          file.pipe(fs.createWriteStream(savePath));
        });
        busboy.on("finish", () => {
          res.writeHead(200, { "Content-Type": "text/plain; charset=utf-8" });
          res.end("Файл загружен");
        });
        req.pipe(busboy);
        return;
      }
    }

    if (req.method !== "GET" && req.method !== "POST") {
      res.writeHead(405, { "Content-Type": "text/plain; charset=utf-8" });
      res.end("Метод не разрешен");
      return;
    }

    res.writeHead(404, { "Content-Type": "text/plain; charset=utf-8" });
    res.end("404 Not Found");
  })
  .listen(3000, () =>
    console.log("Server 08-00 running at http://localhost:3000")
  );
