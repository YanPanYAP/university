const express = require('express');
const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const app = express();
const PORT = Number(process.env.PORT) || 3001;
const HOST = process.env.HOST || '127.0.0.1';
const RESOURCE_PATH = path.join(__dirname, '..', 'resources', 'student.txt');
const sessions = new Map();

app.use(express.json());

function fail409(res, message) {
  res.status(409).json({ error: message });
}

function createSharedKey(secret) {
  return crypto.createHash('sha256').update(secret).digest();
}

function createProof(sessionId, key) {
  return crypto.createHash('sha256').update(`${sessionId}:${key.toString('hex')}`).digest('hex');
}

app.get('/', (req, res) => {
  const dh = crypto.getDiffieHellman('modp15');
  dh.generateKeys();

  const sessionId = crypto.randomUUID();
  const challenge = crypto.randomBytes(16).toString('hex');

  sessions.set(sessionId, {
    step: 'await-client-key',
    dh,
    serverPublicKey: dh.getPublicKey('hex'),
    challenge,
    createdAt: Date.now()
  });

  res.json({
    message: 'DH init',
    sessionId,
    algorithm: 'Diffie-Hellman',
    prime: dh.getPrime('hex'),
    generator: dh.getGenerator('hex'),
    serverPublicKey: dh.getPublicKey('hex'),
    challenge
  });
});

app.post('/keyexchange', (req, res) => {
  const { sessionId, clientPublicKey, challenge } = req.body || {};
  const session = sessions.get(sessionId);

  if (!session || session.step !== 'await-client-key') {
    return fail409(res, 'Invalid handshake state');
  }

  if (
    typeof clientPublicKey !== 'string' ||
    !clientPublicKey.length ||
    typeof challenge !== 'string' ||
    challenge !== session.challenge
  ) {
    return fail409(res, 'Protocol violation during key exchange');
  }

  try {
    const secret = session.dh.computeSecret(clientPublicKey, 'hex');
    const key = createSharedKey(secret);

    session.sharedKey = key;
    session.proof = createProof(sessionId, key);
    session.step = 'ready';

    res.json({
      message: 'Session key created',
      sessionId,
      proof: session.proof,
      cipher: 'aes-256-gcm'
    });
  } catch (error) {
    return fail409(res, 'Failed to compute shared secret');
  }
});

app.get('/resource', (req, res) => {
  const sessionId = req.query.sessionId;
  const proof = req.header('x-session-proof');
  const session = sessions.get(sessionId);

  if (!session || session.step !== 'ready') {
    return fail409(res, 'Resource is unavailable before handshake completion');
  }

  if (typeof proof !== 'string' || proof !== session.proof) {
    return fail409(res, 'Invalid session proof');
  }

  const plaintext = fs.readFileSync(RESOURCE_PATH);
  const iv = crypto.randomBytes(12);
  const cipher = crypto.createCipheriv('aes-256-gcm', session.sharedKey, iv);
  const encrypted = Buffer.concat([cipher.update(plaintext), cipher.final()]);
  const tag = cipher.getAuthTag();

  session.step = 'completed';

  res.json({
    fileName: 'student.encrypted.txt',
    algorithm: 'aes-256-gcm',
    iv: iv.toString('hex'),
    tag: tag.toString('hex'),
    payload: encrypted.toString('hex')
  });
});

app.use((req, res) => {
  res.status(404).json({ error: 'Not found' });
});

app.listen(PORT, HOST, () => {
  console.log(`23-01 server is running at http://${HOST}:${PORT}`);
});
