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
  const portRaw = flags.port || positional[1];
  const port = Number(portRaw);

  return { nick, port };
}

const { nick, port } = readArgs(process.argv);

if (!nick || !Number.isInteger(port) || port <= 0 || port > 65535) {
  console.error('Usage: node server.js <Nick> <Port>');
  console.error('Example: node server.js X 3001');
  process.exit(1);
}

const app = express();
app.use(express.json());

const sendResponse = (method) => (_req, res) => {
  res.json({ Nick: nick, Method: method });
};

app.get('/A', sendResponse('GET'));
app.post('/A', sendResponse('POST'));
app.put('/A', sendResponse('PUT'));
app.delete('/A', sendResponse('DELETE'));

app.listen(port, () => {
  console.log(`TDWA04-01 started: Nick=${nick}, Port=${port}`);
});
