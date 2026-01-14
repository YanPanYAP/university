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
    console.log("square(3) =", await call("square", [3]));
    console.log("square(5,4) =", await call("square", [5, 4]));
    console.log("sum(2) =", await call("sum", [2]));
    console.log("sum(2,4,6,8,10) =", await call("sum", [2, 4, 6, 8, 10]));
    console.log("mul(3) =", await call("mul", [3]));
    console.log(
      "mul(3,5,7,9,11,13) =",
      await call("mul", [3, 5, 7, 9, 11, 13])
    );
    console.log("fib(1) =", await call("fib", [1]));
    console.log("fib(2) =", await call("fib", [2]));
    console.log("fib(7) =", await call("fib", [7]));
    console.log("fact(0) =", await call("fact", [0]));
    console.log("fact(5) =", await call("fact", [5]));
    console.log("fact(10) =", await call("fact", [10]));
  } catch (e) {
    console.error("RPC error", e);
  } finally {
    ws.close();
  }
});
