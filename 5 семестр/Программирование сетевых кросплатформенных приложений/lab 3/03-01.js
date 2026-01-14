const http = require("http");
const readline = require("readline");

const PORT = 3000;
let state = "norm";

const server = http.createServer((req, res) => {
  if (req.url === "/") {
    res.writeHead(200, { "Content-Type": "text/html; charset=utf-8" });
    res.end(`<html><head><meta charset="utf-8"><title>State</title></head>
      <body style="font-family: Georgia, serif; font-size: 48px; margin:20px;">
        ${state}
      </body></html>`);
  } else {
    res.writeHead(404);
    res.end("Not found");
  }
});

server.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}/`);
  showPrompt();
});

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

function showPrompt() {
  process.stdout.write(`${state}->`);
}

rl.on("line", (line) => {
  const cmd = line.trim();
  if (cmd === "") {
    showPrompt();
    return;
  }
  if (cmd === "exit") {
    console.log("Exiting...");
    rl.close();
    server.close(() => process.exit(0));
    return;
  }

  const allowed = new Set(["norm", "stop", "test", "idle"]);
  if (allowed.has(cmd)) {
    state = cmd;
    console.log(`reg = ${cmd}`);
  } else {
    console.log(cmd);
  }
  showPrompt();
});
