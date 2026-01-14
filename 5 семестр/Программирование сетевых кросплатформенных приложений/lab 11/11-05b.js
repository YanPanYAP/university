const { WebSocket } = require("ws");
const async = require("async");
const ws = new WebSocket("ws://localhost:3000");

let nextId = 1;
function call(method, params, callback) {
  const id = (nextId++).toString();
  const onMsg = (m) => {
    try {
      const obj = JSON.parse(m.toString());
      if (obj.id == id) {
        ws.off("message", onMsg);
        if ("error" in obj) callback(obj.error);
        else callback(null, obj.result);
      }
    } catch (e) {}
  };

  ws.on("message", onMsg);
  ws.send(JSON.stringify({ id, method, params }));
}

ws.on("open", () => {
  async.parallel(
    {
      "square(3)": (cb) => call("square", [3], cb),
      "square(5,4)": (cb) => call("square", [5, 4], cb),
      "sum(2)": (cb) => call("sum", [2], cb),
      "sum(2,4,6,8,10)": (cb) => call("sum", [2, 4, 6, 8, 10], cb),
      "mul(3)": (cb) => call("mul", [3], cb),
      "mul(3,5,7,9,11,13)": (cb) => call("mul", [3, 5, 7, 9, 11, 13], cb),
      "fib(1)": (cb) => call("fib", [1], cb),
      "fib(2)": (cb) => call("fib", [2], cb),
      "fib(7)": (cb) => call("fib", [7], cb),
      "fact(0)": (cb) => call("fact", [0], cb),
      "fact(5)": (cb) => call("fact", [5], cb),
      "fact(10)": (cb) => call("fact", [10], cb),
    },
    (err, results) => {
      if (err) {
        console.error("RPC error:", err);
      } else {
        for (const k in results) {
          console.log(k, "=", results[k]);
        }
      }
      ws.close();
    }
  );
});
