const http = require("http");
const querystring = require("querystring");

const postData = querystring.stringify({ x: 2, y: 3, s: "test" });

const options = {
  hostname: "localhost",
  port: 3003,
  path: "/",
  method: "POST",
  headers: {
    "Content-Type": "application/x-www-form-urlencoded",
    "Content-Length": Buffer.byteLength(postData),
  },
};

const req = http.request(options, (res) => {
  let body = "";
  res.setEncoding("utf8");
  res.on("data", (chunk) => (body += chunk));
  res.on("end", () => {
    console.log("Status code:", res.statusCode);
    console.log("Body:", body);
  });
});

req.on("error", console.error);
req.write(postData);
req.end();
