const http = require("http");

const xml = `
<request id="123">
  <x value="10" />
  <x value="20" />
  <m value="Hello" />
  <m value="World" />
</request>
`;

const options = {
  hostname: "localhost",
  port: 3005,
  path: "/xml",
  method: "POST",
  headers: {
    "Content-Type": "application/xml",
    "Content-Length": Buffer.byteLength(xml),
  },
};

const req = http.request(options, (res) => {
  let body = "";
  res.setEncoding("utf8");

  res.on("data", (chunk) => (body += chunk));
  res.on("end", () => {
    console.log("Status:", res.statusCode);
    console.log("Body:\n", body);
  });
});

req.on("error", console.error);

req.write(xml);
req.end();
