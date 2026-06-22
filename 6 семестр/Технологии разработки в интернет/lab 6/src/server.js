const fs = require('fs');
const path = require('path');
const express = require('express');
const { connectWithRetry, getPool, sql } = require('./db');

const envPath = path.join(__dirname, '..', '.env');
if (fs.existsSync(envPath)) {
  require('dotenv').config({ path: envPath });
}

const app = express();
app.use(express.json());

app.get('/health', async (_req, res) => {
  try {
    const result = await getPool().request().query('SELECT 1 AS ok');
    res.json({ ok: result.recordset[0].ok === 1 });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/celebrities', async (_req, res) => {
  try {
    const result = await getPool().request().query(`
      SELECT Id, FullName, Nationality, ReqPhotoPath
      FROM dbo.Celebrities
      ORDER BY Id
    `);
    res.json(result.recordset);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

app.get('/api/celebrities/:id', async (req, res) => {
  try {
    const id = Number(req.params.id);
    if (!Number.isInteger(id) || id <= 0) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const result = await getPool().request()
      .input('Id', sql.Int, id)
      .query('SELECT Id, FullName, Nationality, ReqPhotoPath FROM dbo.Celebrities WHERE Id = @Id');

    if (result.recordset.length === 0) {
      return res.status(404).json({ error: 'Celebrity not found' });
    }

    return res.json(result.recordset[0]);
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

app.post('/api/celebrities', async (req, res) => {
  try {
    const { FullName, Nationality, ReqPhotoPath = null } = req.body;
    if (!FullName || typeof FullName !== 'string' || !Nationality || typeof Nationality !== 'string') {
      return res.status(400).json({ error: 'FullName and Nationality are required strings' });
    }

    if (Nationality.length !== 2) {
      return res.status(400).json({ error: 'Nationality must be 2 characters' });
    }

    const result = await getPool().request()
      .input('FullName', sql.NVarChar(50), FullName)
      .input('Nationality', sql.NVarChar(2), Nationality)
      .input('ReqPhotoPath', sql.NVarChar(200), ReqPhotoPath)
      .query(`
        INSERT INTO dbo.Celebrities (FullName, Nationality, ReqPhotoPath)
        OUTPUT INSERTED.Id, INSERTED.FullName, INSERTED.Nationality, INSERTED.ReqPhotoPath
        VALUES (@FullName, @Nationality, @ReqPhotoPath)
      `);

    return res.status(201).json(result.recordset[0]);
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

app.put('/api/celebrities/:id', async (req, res) => {
  try {
    const id = Number(req.params.id);
    const { FullName, Nationality, ReqPhotoPath = null } = req.body;

    if (!Number.isInteger(id) || id <= 0) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    if (!FullName || typeof FullName !== 'string' || !Nationality || typeof Nationality !== 'string') {
      return res.status(400).json({ error: 'FullName and Nationality are required strings' });
    }

    if (Nationality.length !== 2) {
      return res.status(400).json({ error: 'Nationality must be 2 characters' });
    }

    const result = await getPool().request()
      .input('Id', sql.Int, id)
      .input('FullName', sql.NVarChar(50), FullName)
      .input('Nationality', sql.NVarChar(2), Nationality)
      .input('ReqPhotoPath', sql.NVarChar(200), ReqPhotoPath)
      .query(`
        UPDATE dbo.Celebrities
        SET FullName = @FullName,
            Nationality = @Nationality,
            ReqPhotoPath = @ReqPhotoPath
        OUTPUT INSERTED.Id, INSERTED.FullName, INSERTED.Nationality, INSERTED.ReqPhotoPath
        WHERE Id = @Id
      `);

    if (result.recordset.length === 0) {
      return res.status(404).json({ error: 'Celebrity not found' });
    }

    return res.json(result.recordset[0]);
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

app.delete('/api/celebrities/:id', async (req, res) => {
  try {
    const id = Number(req.params.id);
    if (!Number.isInteger(id) || id <= 0) {
      return res.status(400).json({ error: 'Invalid id' });
    }

    const result = await getPool().request()
      .input('Id', sql.Int, id)
      .query('DELETE FROM dbo.Celebrities OUTPUT DELETED.Id WHERE Id = @Id');

    if (result.recordset.length === 0) {
      return res.status(404).json({ error: 'Celebrity not found' });
    }

    return res.status(204).send();
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

async function start() {
  await connectWithRetry();
  const port = Number(process.env.APP_PORT || 3006);
  app.listen(port, () => {
    const dbHost = process.env.DB_HOST || '127.0.0.1';
    const dbPort = process.env.DB_PORT || '14333';
    console.log(`TDWA06-01 started on port ${port}. DB: ${dbHost}:${dbPort}`);
  });
}

start().catch((error) => {
  console.error('Failed to start TDWA06-01:', error.message);
  process.exit(1);
});
