const http = require("http");
const querystring = require("querystring");

const srv = http.createServer((req, res) => {
  if (req.method !== "POST") return res.end("Use POST");
  let body = "";
  req.on("data", (chunk) => (body += chunk));
  req.on("end", () => {
    const params = querystring.parse(body);
    res.writeHead(200, { "Content-Type": "application/json" });
    res.end(JSON.stringify(params));
  });
});

srv.listen(3003, () =>
  console.log("09-03 server listening on http://localhost:3003")
);
