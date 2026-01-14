const http = require("http");
const url = require("url");
const xml2js = require("xml2js");
const builder = require("xmlbuilder");

const server = http.createServer((req, res) => {
  const { pathname } = url.parse(req.url, true);

  if (pathname === "/xml" && req.method === "POST") {
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

        const request = result.request;

        const requestId = request.$?.id || "unknown";

        let xElems = Array.isArray(request.x) ? request.x : [request.x];
        let xValues = xElems.map((x) => parseFloat(x.$?.value) || 0);

        let mElems = Array.isArray(request.m) ? request.m : [request.m];
        let mValues = mElems.map((m) => m.$?.value || "");

        let numericSum = xValues.reduce((a, b) => a + b, 0);
        let concat = mValues.join(" ");

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

  res.writeHead(404);
  res.end("Not found");
});

server.listen(3005, () =>
  console.log("XML server running on http://localhost:3005/xml")
);
