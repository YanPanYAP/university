const http = require("http");
const url = require("url");

const x = 5;
const y = 7;
const requestUrl = `http://localhost:3002/?x=${x}&y=${y}`;

http
  .get(requestUrl, (res) => {
    let body = "";
    res.setEncoding("utf8");
    res.on("data", (d) => (body += d));
    res.on("end", () => {
      console.log("Status code:", res.statusCode);
      console.log("Body:", body);
    });
  })
  .on("error", console.error);
