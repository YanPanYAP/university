const express = require("express");

const app = express();
const PORT = Number(process.env.PORT || 3000);

app.use(express.json({ strict: true }));

function toNumber(value) {
  if (typeof value !== "number" || !Number.isFinite(value)) {
    return null;
  }
  return value;
}

function invalidParams(message) {
  return { code: -32602, message, data: null };
}

function getVariadicParams(params) {
  const values = Array.isArray(params)
    ? params
    : params && typeof params === "object"
      ? Object.values(params)
      : null;

  if (!values || values.length === 0) {
    return { error: invalidParams("Expected one or more numeric params") };
  }

  const numbers = [];
  for (const value of values) {
    const n = toNumber(value);
    if (n === null) {
      return { error: invalidParams("All params must be finite numbers") };
    }
    numbers.push(n);
  }

  return { numbers };
}

function getXYParams(params) {
  let x;
  let y;

  if (Array.isArray(params)) {
    if (params.length !== 2) {
      return { error: invalidParams("Expected exactly two params: x, y") };
    }
    [x, y] = params;
  } else if (params && typeof params === "object") {
    x = params.x;
    y = params.y;
  } else {
    return { error: invalidParams("Expected params as array [x, y] or object {x, y}") };
  }

  const xn = toNumber(x);
  const yn = toNumber(y);

  if (xn === null || yn === null) {
    return { error: invalidParams("x and y must be finite numbers") };
  }

  if (yn === 0) {
    return { error: invalidParams("Division by zero is not allowed") };
  }

  return { x: xn, y: yn };
}

const methods = {
  sum(params) {
    const parsed = getVariadicParams(params);
    if (parsed.error) {
      throw parsed.error;
    }
    return parsed.numbers.reduce((acc, n) => acc + n, 0);
  },
  mul(params) {
    const parsed = getVariadicParams(params);
    if (parsed.error) {
      throw parsed.error;
    }
    return parsed.numbers.reduce((acc, n) => acc * n, 1);
  },
  div(params) {
    const parsed = getXYParams(params);
    if (parsed.error) {
      throw parsed.error;
    }
    return parsed.x / parsed.y;
  },
  proc(params) {
    const parsed = getXYParams(params);
    if (parsed.error) {
      throw parsed.error;
    }
    return (parsed.x / parsed.y) * 100;
  }
};

function makeResponse(id, result, error) {
  const payload = { jsonrpc: "2.0", id: id ?? null };
  if (error) {
    payload.error = error;
  } else {
    payload.result = result;
  }
  return payload;
}

function handleRpcCall(call) {
  if (!call || typeof call !== "object" || Array.isArray(call)) {
    return makeResponse(null, null, { code: -32600, message: "Invalid Request", data: null });
  }

  const { jsonrpc, method, params, id } = call;
  const isNotification = typeof id === "undefined";

  if (jsonrpc !== "2.0" || typeof method !== "string" || method.length === 0) {
    return isNotification
      ? null
      : makeResponse(id ?? null, null, { code: -32600, message: "Invalid Request", data: null });
  }

  const fn = methods[method];
  if (!fn) {
    return isNotification
      ? null
      : makeResponse(id, null, { code: -32601, message: "Method not found", data: null });
  }

  try {
    const result = fn(params);
    if (isNotification) {
      return null;
    }
    return makeResponse(id, result, null);
  } catch (error) {
    if (isNotification) {
      return null;
    }
    if (error && typeof error.code === "number" && typeof error.message === "string") {
      return makeResponse(id, null, error);
    }
    return makeResponse(id, null, { code: -32603, message: "Internal error", data: null });
  }
}

app.post("/jsonrpc", (req, res) => {
  const body = req.body;

  if (Array.isArray(body)) {
    if (body.length === 0) {
      return res
        .status(400)
        .json(makeResponse(null, null, { code: -32600, message: "Invalid Request", data: null }));
    }

    const responses = body.map(handleRpcCall).filter(Boolean);
    if (responses.length === 0) {
      return res.status(204).send();
    }

    return res.status(200).json(responses);
  }

  const response = handleRpcCall(body);
  if (!response) {
    return res.status(204).send();
  }

  return res.status(200).json(response);
});

app.use((err, req, res, next) => {
  if (err instanceof SyntaxError && "body" in err) {
    return res
      .status(400)
      .json(makeResponse(null, null, { code: -32700, message: "Parse error", data: null }));
  }

  return res
    .status(500)
    .json(makeResponse(null, null, { code: -32603, message: "Internal error", data: null }));
});

app.use((req, res) => {
  res.status(404).json({ error: "Route not found. Use POST /jsonrpc" });
});

app.listen(PORT, () => {
  console.log(`25-01 JSON-RPC server started on port ${PORT}`);
});
