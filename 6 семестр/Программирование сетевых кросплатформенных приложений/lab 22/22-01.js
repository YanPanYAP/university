const https = require('https');
const fs = require('fs');
const path = require('path');

const PORT = Number(process.env.PORT) || 3443;
const HOST = process.env.HOST || '127.0.0.1';

const keyPath = process.env.TLS_KEY || path.join(__dirname, 'certs', 'resource.key.pem');
const certPath = process.env.TLS_CERT || path.join(__dirname, 'certs', 'resource.crt.pem');

if (!fs.existsSync(keyPath) || !fs.existsSync(certPath)) {
  console.error('TLS files not found.');
  console.error(`Expected key: ${keyPath}`);
  console.error(`Expected cert: ${certPath}`);
  process.exit(1);
}

const tlsOptions = {
  key: fs.readFileSync(keyPath),
  cert: fs.readFileSync(certPath)
};

const server = https.createServer(tlsOptions, (req, res) => {
  if (req.method !== 'GET') {
    res.writeHead(405, { 'Content-Type': 'text/plain; charset=utf-8' });
    res.end('Method Not Allowed: only GET is supported');
    return;
  }

  res.writeHead(200, { 'Content-Type': 'text/html; charset=utf-8' });
  res.end(`
    <h1>LAB 22 HTTPS Resource</h1>
    <p>Method: ${req.method}</p>
    <p>Path: ${req.url}</p>
    <p>Time: ${new Date().toISOString()}</p>
  `);
});

server.listen(PORT, HOST, () => {
  console.log(`HTTPS server for lab 22 started at https://localhost:${PORT}`);
});
