import { useState } from "react";
import "./App.css";

function App() {
  const [op, setOp] = useState("");
  const [x, setX] = useState("");
  const [y, setY] = useState("");
  const [result, setResult] = useState("");

  const url = "https://localhost:20443/api/Save-JSON";

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
    <div className="page">
      <h1>TDWA03-01 SPA</h1>

      <div className="form">
        <input
          placeholder="op"
          value={op}
          onChange={(e) => setOp(e.target.value)}
        />
        <input
          type="number"
          placeholder="x"
          value={x}
          onChange={(e) => setX(e.target.value)}
        />
        <input
          type="number"
          placeholder="y"
          value={y}
          onChange={(e) => setY(e.target.value)}
        />
      </div>

      <div className="buttons">
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
