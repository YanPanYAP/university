import { useEffect, useState, KeyboardEvent, useCallback } from "react";
import "./index.css";

type Theme = "light" | "dark";

interface HistoryItem {
  expression: string;
  result: string;
}

interface ButtonProps {
  value: string;
  onClick: (value: string) => void;
  className?: string;
}

const Button = ({ value, onClick, className = "" }: ButtonProps) => (
  <button className={className} onClick={() => onClick(value)}>
    {value === "*" ? "×" : value === "/" ? "÷" : value}
  </button>
);

const Display = ({ input }: { input: string }) => (
  <div className="display">{input}</div>
);

const History = ({ history }: { history: HistoryItem[] }) => (
  <div className="technologies-used">
    <p>
      <strong>История</strong>
    </p>
    {history
      .slice(-5)
      .reverse()
      .map((item, index) => (
        <p key={index}>
          {item.expression} = {item.result}
        </p>
      ))}
  </div>
);

function Calculator() {
  const [input, setInput] = useState("0");
  const [history, setHistory] = useState<HistoryItem[]>([]);
  const [theme, setTheme] = useState<Theme>("dark");
  const [justEvaluated, setJustEvaluated] = useState(false);
  const [lastOperator, setLastOperator] = useState<string | null>(null);
  const [lastOperand, setLastOperand] = useState<string | null>(null);

  const MAX_LENGTH = 18;
  const keys = "0123456789/*-+.";

  const isOperator = (val: string) => ["+", "-", "*", "/"].includes(val);

  const handleClick = useCallback(
    (value: string) => {
      if (value === "C") {
        setInput("0");
        setJustEvaluated(false);
        setLastOperator(null);
        setLastOperand(null);
        return;
      }

      if (value === "⌫") {
        setInput((prev) => (prev.length > 1 ? prev.slice(0, -1) : "0"));
        setJustEvaluated(false);
        return;
      }

      if (value === "=") {
        if (justEvaluated) return;

        try {
          const sanitized = input.replace(/[^-()\d/*+.]/g, "");
          const result =
            lastOperator && lastOperand
              ? eval(`${input}${lastOperator}${lastOperand}`)
              : eval(sanitized);

          if (!isFinite(result)) throw new Error("Деление на ноль");

          const formatted = parseFloat(result.toFixed(3)).toString();

          if (input !== formatted) {
            setHistory((prev) => [
              ...prev,
              { expression: input, result: formatted },
            ]);
          }

          setInput(formatted);
          setJustEvaluated(true);
          setLastOperator(null);
          setLastOperand(null);
        } catch {
          setInput("Ошибка");
          setJustEvaluated(false);
        }

        return;
      }

      if (value === "+/-") {
        setInput((prev) => toggleSign(prev));
        setJustEvaluated(false);
        return;
      }

      setInput((prev) => {
        if (prev === "Ошибка") return value;

        if (justEvaluated) {
          if (isOperator(value)) {
            setJustEvaluated(false);
            return prev + value;
          } else {
            setJustEvaluated(false);
            setLastOperator(null);
            setLastOperand(null);
            return value === "." ? "0." : value;
          }
        }

        if (prev === "0" && !isNaN(+value)) return value;
        if (prev.length >= MAX_LENGTH) return prev;

        const lastChar = prev.slice(-1);
        if (isOperator(lastChar) && isOperator(value)) {
          return prev.slice(0, -1) + value;
        }

        if (lastChar === "." && value === ".") return prev;

        return prev + value;
      });

      if (!isOperator(value)) setJustEvaluated(false);
    },
    [input, justEvaluated, lastOperator, lastOperand]
  );

  const toggleSign = (expr: string): string => {
    const match = expr.match(/([+-]?\d+(\.\d+)?)(?!.*\d)/);
    if (!match) return expr;

    const number = match[0];
    const start = match.index!;
    const before = expr[start - 1];

    if (parseFloat(number) === 0) return expr;

    if (start === 0) {
      if (number.startsWith("-") || number.startsWith("+")) {
        return number.slice(1) + expr.slice(number.length);
      } else {
        return "-" + expr;
      }
    }

    if (before === "+" || before === "-") {
      return (
        expr.slice(0, start - 1) +
        (before === "+" ? "-" : "+") +
        expr.slice(start)
      );
    }

    if (number.startsWith("+") || number.startsWith("-")) {
      const toggled = number.startsWith("-")
        ? "+" + number.slice(1)
        : "-" + number.slice(1);
      return expr.slice(0, start) + toggled + expr.slice(start + number.length);
    }

    return (
      expr.slice(0, start) + "-" + number + expr.slice(start + number.length)
    );
  };

  const handleKeyDown = (e: KeyboardEvent<HTMLDivElement>) => {
    if (keys.includes(e.key)) handleClick(e.key);
    else if (e.key === "Enter") handleClick("=");
    else if (e.key === "Backspace") handleClick("⌫");
    else if (e.key === "Escape") handleClick("C");
  };

  const toggleTheme = () => {
    const newTheme = theme === "dark" ? "light" : "dark";
    setTheme(newTheme);
  };

  useEffect(() => {
    document.body.tabIndex = 0;
    document.body.focus();
    document.body.classList.remove(theme === "dark" ? "light" : "dark");
    document.body.classList.add(theme);
  }, [theme]);

  const changeByOne = (delta: number) => {
    try {
      const result = parseFloat(input);
      const newValue = (result + delta).toString();
      const expression = `${input} ${delta > 0 ? "+" : "-"} 1`;

      setInput(newValue);
      setHistory((prev) => [...prev, { expression, result: newValue }]);
      setJustEvaluated(true);
      setLastOperator(null);
      setLastOperand(null);
    } catch {
      setInput("Ошибка");
      setJustEvaluated(false);
    }
  };

  useEffect(() => {
    document.body.tabIndex = 0;
    document.body.focus();
  }, []);

  return (
    <div
      className="calculator-container"
      onKeyDown={handleKeyDown}
      tabIndex={0}
    >
      <h1 className="calculator-title">Calculator</h1>
      <div className="calculator">
        <Display input={input} />

        <div className="increment-buttons">
          <button className="increment" onClick={() => changeByOne(1)}>
            +1
          </button>
          <button className="decrement" onClick={() => changeByOne(-1)}>
            -1
          </button>
        </div>

        <div className="buttons">
          {["1", "2", "3", "+"].map((val) => (
            <Button
              key={val}
              value={val}
              onClick={handleClick}
              className={isOperator(val) ? "operator" : ""}
            />
          ))}
          {["4", "5", "6", "-"].map((val) => (
            <Button
              key={val}
              value={val}
              onClick={handleClick}
              className={isOperator(val) ? "operator" : ""}
            />
          ))}
          {["7", "8", "9", "*"].map((val) => (
            <Button
              key={val}
              value={val}
              onClick={handleClick}
              className={isOperator(val) ? "operator" : ""}
            />
          ))}
          {["0", ".", "=", "/"].map((val) => (
            <Button
              key={val}
              value={val}
              onClick={handleClick}
              className={
                val === "=" ? "equals" : isOperator(val) ? "operator" : ""
              }
            />
          ))}
          <Button value={theme === "dark" ? "☽" : "☀"} onClick={toggleTheme} />
          <Button value="+/-" onClick={handleClick} />
          <Button value="C" onClick={handleClick} className="clear" />
          <Button value="⌫" onClick={handleClick} />
        </div>
      </div>
      <History history={history} />
    </div>
  );
}

export default Calculator;
