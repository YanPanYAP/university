const http = require("http");
const url = require("url");

const options = url.parse("http://localhost:3001");

const req = http.get(options, (res) => {
  const { statusCode, statusMessage } = res;
  const remoteIP = res.socket.remoteAddress;
  const remotePort = res.socket.remotePort;
  let body = "";
  res.setEncoding("utf8");
  res.on("data", (chunk) => (body += chunk));
  res.on("end", () => {
    console.log("Status code:", statusCode);
    console.log("Status message:", statusMessage);
    console.log("Remote IP:", remoteIP);
    console.log("Remote port:", remotePort);
    console.log("Body:", body);
  });
});

req.on("error", (e) => console.error(e));
