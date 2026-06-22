const express = require('express');
const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const app = express();
const PORT = Number(process.env.PORT) || 3002;
const HOST = process.env.HOST || '127.0.0.1';
const RESOURCE_PATH = path.join(__dirname, '..', 'resources', 'student.txt');
const sessions = new Map();

const { privateKey, publicKey } = crypto.generateKeyPairSync('rsa', {
  modulusLength: 2048
});

app.use(express.json());

function fail409(res, message) {
  res.status(409).json({ error: message });
}

app.get('/', (req, res) => {
  const sessionId = crypto.randomUUID();
  const challenge = crypto.randomBytes(16).toString('hex');

  sessions.set(sessionId, {
    step: 'await-ack',
    challenge
  });

  res.json({
    message: 'Signature handshake init',
    sessionId,
    challenge
  });
});

app.post('/ack', (req, res) => {
  const { sessionId, challenge, clientNonce } = req.body || {};
  const session = sessions.get(sessionId);

  if (!session || session.step !== 'await-ack') {
    return fail409(res, 'Invalid handshake state');
  }

  if (
    typeof challenge !== 'string' ||
    challenge !== session.challenge ||
    typeof clientNonce !== 'string' ||
    clientNonce.length < 8
  ) {
    return fail409(res, 'Protocol violation during acknowledgement');
  }

  session.step = 'ready';
  session.clientNonce = clientNonce;

  res.json({
    message: 'Handshake accepted',
    sessionId
  });
});

app.get('/resource', (req, res) => {
  const { sessionId, clientNonce } = req.query;
  const session = sessions.get(sessionId);

  if (!session || session.step !== 'ready') {
    return fail409(res, 'Resource is unavailable before handshake completion');
  }

  if (clientNonce !== session.clientNonce) {
    return fail409(res, 'Invalid client nonce');
  }

  const text = fs.readFileSync(RESOURCE_PATH, 'utf8');
  const signature = crypto.sign('sha256', Buffer.from(text, 'utf8'), privateKey);

  session.step = 'completed';

  res.json({
    fileName: 'student.txt',
    content: text,
    signature: signature.toString('base64'),
    signatureAlgorithm: 'RSA-SHA256',
    publicKey: publicKey.export({ type: 'spki', format: 'pem' })
  });
});

app.use((req, res) => {
  res.status(404).json({ error: 'Not found' });
});

app.listen(PORT, HOST, () => {
  console.log(`23-02 server is running at http://${HOST}:${PORT}`);
});
