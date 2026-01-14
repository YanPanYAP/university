const fs = require("fs");
const path = require("path");

module.exports = function (staticDirName) {
  const mimeTypes = {
    html: "text/html",
    css: "text/css",
    js: "text/javascript",
    png: "image/png",
    docx: "application/msword",
    json: "application/json",
    xml: "application/xml",
    mp4: "video/mp4",
  };

  return function handleRequest(req, res) {
    if (req.method !== "GET") {
      res.statusCode = 405;
      res.end("405 error");
      return;
    }

    let filePath = decodeURI(req.url);

    if (filePath === "/") filePath = "/index.html";

    const fullPath = path.join(__dirname, staticDirName, filePath);

    if (!fullPath.startsWith(path.join(__dirname, staticDirName))) {
      res.statusCode = 404;
      res.end("404 not found");
      return;
    }

    fs.stat(fullPath, (err, stats) => {
      if (err || !stats.isFile()) {
        res.statusCode = 404;
        res.end("404 not found");
        return;
      }

      const ext = path.extname(fullPath).substring(1);
      const mime = mimeTypes[ext];

      if (!mime) {
        res.statusCode = 404;
        res.end("404 error type");
        return;
      }

      res.writeHead(200, { "Content-Type": mime });

      fs.createReadStream(fullPath).pipe(res);
    });
  };
};
