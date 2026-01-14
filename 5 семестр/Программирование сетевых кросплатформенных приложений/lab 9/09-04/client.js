const http = require("http");

const payload = {
  x: 5,
  y: 7,
  s: "bello",
  m: [1, 2, 3],
  o: { a: "one", b: "two" },
};

const postData = JSON.stringify(payload);

const options = {
  hostname: "localhost",
  port: 3004,
  path: "/json",
  method: "POST",
  headers: {
    "Content-Type": "application/json",
    "Content-Length": Buffer.byteLength(postData),
  },
};

const req = http.request(options, (res) => {
  let body = "";
  res.setEncoding("utf8");

  res.on("data", (chunk) => (body += chunk));
  res.on("end", () => {
    console.log("Status:", res.statusCode);
    try {
      const parsed = JSON.parse(body);
      console.log("Ответ сервера:", parsed);
    } catch {
      console.log("Raw:", body);
    }
  });
});

req.on("error", console.error);

req.write(postData);
req.end();
