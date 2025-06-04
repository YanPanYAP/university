import { useState } from "react";
import "./App.css";

function Button({ title, onClick, disabled = false }) {
  return (
    <button onClick={onClick} disabled={disabled} className="button">
      {title}
    </button>
  );
}

function Counter() {
  const [count, setCount] = useState(0);

  function handleIncrease() {
    setCount((prev) => prev + 1);
  }

  function handleReset() {
    setCount(0);
  }

  return (
    <div className="mainDiv">
      <h1 style={{ color: "skyblue" }}>{count}</h1>
      <div className="space">
        <Button title="inc" onClick={handleIncrease} disabled={count === 5} />
        <Button title="reset" onClick={handleReset} disabled={count === 0} />
      </div>
    </div>
  );
}

function App() {
  return (
    <>
      <Counter />
    </>
  );
}

export default App;
