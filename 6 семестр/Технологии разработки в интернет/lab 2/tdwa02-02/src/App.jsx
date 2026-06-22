import { useState } from "react";

function App() {
  const [op, setOp] = useState("");
  const [x, setX] = useState("");
  const [y, setY] = useState("");
  const [result, setResult] = useState("");

  const url = "/api/Save-JSON";

  const send = async (method) => {
    const options = {
      method,
      headers: { "Content-Type": "application/json" }
    };

    if (method !== "GET" && method !== "DELETE") {
      options.body = JSON.stringify({
        op,
        x: Number(x),
        y: Number(y)
      });
    }

    const res = await fetch(url, options);
    const text = await res.text();
    setResult(text);
  };

  return (
    <div style={{ padding: 40 }}>
      <h1>TDWA02-02 SPA</h1>

      <input placeholder="op" onChange={e => setOp(e.target.value)} />
      <input type="number" placeholder="x" onChange={e => setX(e.target.value)} />
      <input type="number" placeholder="y" onChange={e => setY(e.target.value)} />

      <div>
        <button onClick={() => send("GET")}>GET</button>
        <button onClick={() => send("POST")}>POST</button>
        <button onClick={() => send("PUT")}>PUT</button>
        <button onClick={() => send("DELETE")}>DELETE</button>
      </div>

      <pre>{result}</pre>
    </div>
  );
}

export default App;
