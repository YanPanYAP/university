const sql = require('mssql');

const config = {
  server: process.env.DB_HOST || '127.0.0.1',
  port: Number(process.env.DB_PORT || 14333),
  user: process.env.DB_USER || 'sa',
  password: process.env.DB_PASSWORD || 'MyPass123',
  database: process.env.DB_NAME || 'master',
  options: {
    encrypt: false,
    trustServerCertificate: true
  },
  pool: {
    max: 10,
    min: 0,
    idleTimeoutMillis: 30000
  }
};

let pool;

async function connectWithRetry(maxRetries = 30, delayMs = 2000) {
  let lastError;
  for (let i = 1; i <= maxRetries; i += 1) {
    try {
      pool = await sql.connect(config);
      return pool;
    } catch (error) {
      lastError = error;
      await new Promise((resolve) => setTimeout(resolve, delayMs));
    }
  }
  throw lastError;
}

function getPool() {
  if (!pool) {
    throw new Error('Database pool is not initialized.');
  }
  return pool;
}

module.exports = {
  sql,
  config,
  connectWithRetry,
  getPool
};
