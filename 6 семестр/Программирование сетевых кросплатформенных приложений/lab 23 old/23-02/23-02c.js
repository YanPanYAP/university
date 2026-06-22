const fs = require('fs');
const path = require('path');
const crypto = require('crypto');

const SERVER_URL = process.env.SERVER_URL || 'http://127.0.0.1:3002';
const OUTPUT_PATH = path.join(__dirname, '..', 'output', '23-02-client-resource.txt');

async function main() {
  const initResponse = await fetch(`${SERVER_URL}/`);
  if (!initResponse.ok) {
    throw new Error(`Handshake init failed: ${initResponse.status}`);
  }

  const init = await initResponse.json();
  const clientNonce = crypto.randomBytes(12).toString('hex');

  const ackResponse = await fetch(`${SERVER_URL}/ack`, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      sessionId: init.sessionId,
      challenge: init.challenge,
      clientNonce
    })
  });

  if (!ackResponse.ok) {
    throw new Error(`Handshake acknowledgement failed: ${ackResponse.status}`);
  }

  const resourceResponse = await fetch(
    `${SERVER_URL}/resource?sessionId=${encodeURIComponent(init.sessionId)}&clientNonce=${encodeURIComponent(clientNonce)}`
  );

  if (!resourceResponse.ok) {
    throw new Error(`Signed resource request failed: ${resourceResponse.status}`);
  }

  const resource = await resourceResponse.json();
  const isValid = crypto.verify(
    'sha256',
    Buffer.from(resource.content, 'utf8'),
    resource.publicKey,
    Buffer.from(resource.signature, 'base64')
  );

  fs.writeFileSync(OUTPUT_PATH, resource.content, 'utf8');

  console.log(`23-02 client saved resource to ${OUTPUT_PATH}`);
  console.log(resource.content);
  console.log(`Signature verification: ${isValid ? 'VALID' : 'INVALID'}`);
}

main().catch((error) => {
  console.error(error.message);
  process.exit(1);
});
