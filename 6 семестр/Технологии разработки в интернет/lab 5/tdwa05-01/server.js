const express = require('express');

function readArgs(argv) {
  const [, , ...rest] = argv;

  const flags = {};
  const positional = [];

  for (const item of rest) {
    if (item.startsWith('--')) {
      const [key, value] = item.slice(2).split('=');
      if (key && value) {
        flags[key.toLowerCase()] = value;
      }
    } else {
      positional.push(item);
    }
  }

  const nick = flags.nick || positional[0];
  const port = Number(flags.port || positional[1]);
  const delay = Number(flags.delay || positional[2]);

  return { nick, port, delay };
}

function wait(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

const { nick, port, delay } = readArgs(process.argv);

if (
  !nick ||
  !Number.isInteger(port) ||
  port <= 0 ||
  port > 65535 ||
  !Number.isFinite(delay) ||
  delay < 0
) {
  console.error('Usage: node server.js <Nick> <Port> <Delay>');
  console.error('Example: node server.js X 3101 1000');
  process.exit(1);
}

const app = express();
app.use(express.json());

const delayMap = {
  GET: Math.floor(delay / 3),
  POST: Math.floor((2 * delay) / 3),
  PUT: Math.floor(delay),
  DELETE: Math.floor(delay / 4)
};

const sendResponse = (method) => async (_req, res) => {
  await wait(delayMap[method]);
  res.json({ Nick: nick, Method: method });
};

app.get('/A', sendResponse('GET'));
app.post('/A', sendResponse('POST'));
app.put('/A', sendResponse('PUT'));
app.delete('/A', sendResponse('DELETE'));

app.listen(port, () => {
  console.log(`TDWA05-01 started: Nick=${nick}, Port=${port}, Delay=${delay}`);
});
