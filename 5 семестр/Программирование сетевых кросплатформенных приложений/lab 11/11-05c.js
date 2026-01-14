const { WebSocket } = require("ws");

const ws = new WebSocket("ws://localhost:3000");
let nextId = 1;
function call(method, params) {
  return new Promise((res, rej) => {
    const id = (nextId++).toString();
    const onMsg = (m) => {
      try {
        const obj = JSON.parse(m.toString());
        if (obj.id == id) {
          ws.off("message", onMsg);
          if ("error" in obj) rej(obj.error);
          else res(obj.result);
        }
      } catch (e) {}
    };
    ws.on("message", onMsg);
    ws.send(JSON.stringify({ id, method, params }));
  });
}

ws.on("open", async () => {
  try {
    const [s1, s2, m1, fib7, m2] = await Promise.all([
      call("square", [3]),
      call("square", [5, 4]),
      call("mul", [3, 5, 7, 9, 11, 13]),
      call("fib", [7]),
      call("mul", [2, 4, 6]),
    ]);
    const sumPart = s1 + s2 + m1;
    const fibSum = Array.isArray(fib7)
      ? fib7.reduce((a, b) => a + b, 0)
      : +fib7 || 0;
    const result = sumPart + fibSum * m2;
    console.log("Computed expression result =", result);
  } catch (e) {
    console.error(e);
  } finally {
    ws.close();
  }
});
