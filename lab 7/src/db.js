'use strict';

const sql = require('mssql');

const config = {
  server: process.env.DB_HOST || 'nginx',
  port: Number(process.env.DB_PORT || 1433),
  user: process.env.DB_USER || 'sa',
  password: process.env.DB_PASSWORD || 'MyPass123',
  database: process.env.DB_NAME || 'Celebrities',
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

async function connectWithRetry(maxRetries = 45, delayMs = 2000) {
  let lastError;
  for (let i = 1; i <= maxRetries; i += 1) {
    try {
      pool = await sql.connect(config);
      return pool;
    } catch (error) {
      lastError = error;
      console.log(`Database is not ready, attempt ${i}/${maxRetries}: ${error.message}`);
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

async function ensureDatabase() {
  const masterConfig = { ...config, database: 'master' };
  const masterPool = await new sql.ConnectionPool(masterConfig).connect();
  try {
    await masterPool.request().batch(`
      IF DB_ID(N'${config.database}') IS NULL
      BEGIN
        CREATE DATABASE [${config.database}];
      END
    `);
  } finally {
    await masterPool.close();
  }

  await connectWithRetry();
  await getPool().request().batch(`
    IF OBJECT_ID(N'dbo.Celebrities', N'U') IS NULL
    BEGIN
      CREATE TABLE [dbo].[Celebrities](
        [Id] [int] IDENTITY(1,1) NOT NULL,
        [FullName] [nvarchar](50) NOT NULL,
        [Nationality] [nvarchar](2) NOT NULL,
        [ReqPhotoPath] [nvarchar](200) NULL,
        CONSTRAINT [PK_Celebrities] PRIMARY KEY CLUSTERED ([Id] ASC)
      );
    END;
  `);
}

module.exports = {
  sql,
  config,
  connectWithRetry,
  ensureDatabase,
  getPool
};
