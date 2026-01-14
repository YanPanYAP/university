const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({ port: 3000 }, () =>
  console.log("11-05 RPC server on ws://localhost:3000")
);

function square(...params) {
  if (params.length === 1) {
    const r = params[0];
    return Math.PI * r * r;
  } else if (params.length === 2) {
    return params[0] * params[1];
  } else throw new Error("square expects 1 or 2 params");
}

function sum(...params) {
  return params.reduce((a, b) => a + (+b || 0), 0);
}
function mul(...params) {
  return params.reduce((a, b) => a * (+b || 0), params.length ? 1 : 0);
}
function fib(n) {
  n = Math.max(0, Math.floor(n));
  const a = [];
  for (let i = 0; i < n; i++) a.push(i < 2 ? i : a[i - 1] + a[i - 2]);
  return a;
}
function fact(n) {
  n = Math.max(0, Math.floor(n));
  let res = 1;
  for (let i = 2; i <= n; i++) res *= i;
  return res;
}

const methods = { square, sum, mul, fib, fact };

wss.on("connection", (ws) => {
  ws.on("message", async (data) => {
    try {
      const req = JSON.parse(data.toString());
      const { id, method, params } = req;
      if (!id || !method) {
        ws.send(JSON.stringify({ id: id || null, error: "invalid request" }));
        return;
      }
      if (!(method in methods)) {
        ws.send(JSON.stringify({ id, error: "method not found" }));
        return;
      }
      try {
        const result = methods[method].apply(
          null,
          Array.isArray(params) ? params : []
        );
        ws.send(JSON.stringify({ id, result }));
      } catch (err) {
        ws.send(JSON.stringify({ id, error: err.message }));
      }
    } catch (e) {
      ws.send(JSON.stringify({ id: null, error: "invalid json" }));
    }
  });
});
