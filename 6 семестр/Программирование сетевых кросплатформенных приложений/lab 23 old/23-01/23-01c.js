const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const SERVER_URL = process.env.SERVER_URL || 'http://127.0.0.1:3001';
const OUTPUT_PATH = path.join(__dirname, '..', 'output', '23-01-client-resource.txt');

function createSharedKey(secret) {
  return crypto.createHash('sha256').update(secret).digest();
}

async function main() {
  const initResponse = await fetch(`${SERVER_URL}/`);
  if (!initResponse.ok) {
    throw new Error(`Handshake init failed: ${initResponse.status}`);
  }

  const init = await initResponse.json();
  const dh = crypto.createDiffieHellman(
    Buffer.from(init.prime, 'hex'),
    Buffer.from(init.generator, 'hex')
  );

  dh.generateKeys();

  const keyResponse = await fetch(`${SERVER_URL}/keyexchange`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      sessionId: init.sessionId,
      clientPublicKey: dh.getPublicKey('hex'),
      challenge: init.challenge
    })
  });

  if (!keyResponse.ok) {
    throw new Error(`Key exchange failed: ${keyResponse.status}`);
  }

  const keyResult = await keyResponse.json();
  const secret = dh.computeSecret(Buffer.from(init.serverPublicKey, 'hex'));
  const sharedKey = createSharedKey(secret);

  const resourceResponse = await fetch(
    `${SERVER_URL}/resource?sessionId=${encodeURIComponent(init.sessionId)}`,
    {
      headers: {
        'x-session-proof': keyResult.proof
      }
    }
  );

  if (!resourceResponse.ok) {
    throw new Error(`Encrypted resource request failed: ${resourceResponse.status}`);
  }

  const resource = await resourceResponse.json();
  const decipher = crypto.createDecipheriv(
    'aes-256-gcm',
    sharedKey,
    Buffer.from(resource.iv, 'hex')
  );
  decipher.setAuthTag(Buffer.from(resource.tag, 'hex'));

  const plaintext = Buffer.concat([
    decipher.update(Buffer.from(resource.payload, 'hex')),
    decipher.final()
  ]);

  fs.writeFileSync(OUTPUT_PATH, plaintext);

  console.log(`23-01 client decrypted resource and saved it to ${OUTPUT_PATH}`);
  console.log(plaintext.toString('utf8'));
}

main().catch((error) => {
  console.error(error.message);
  process.exit(1);
});
