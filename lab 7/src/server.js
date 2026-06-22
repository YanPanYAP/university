'use strict';

const express = require('express');
const { ensureDatabase, getPool, sql } = require('./db');

const app = express();
app.use(express.json());

function validateCelebrity(body) {
  const { FullName, Nationality, ReqPhotoPath = null } = body;
  if (!FullName || typeof FullName !== 'string') return 'FullName is required';
  if (FullName.length > 50) return 'FullName length must be <= 50';
  if (!Nationality || typeof Nationality !== 'string' || Nationality.length !== 2) return 'Nationality must be 2 characters';
  if (ReqPhotoPath !== null && (typeof ReqPhotoPath !== 'string' || ReqPhotoPath.length > 200)) {
    return 'ReqPhotoPath must be null or string with length <= 200';
  }
  return null;
}

function addCelebrityInputs(request, body) {
  request
    .input('FullName', sql.NVarChar(50), body.FullName.trim())
    .input('Nationality', sql.NVarChar(2), body.Nationality.trim().toUpperCase())
    .input('ReqPhotoPath', sql.NVarChar(200), body.ReqPhotoPath || null);
}

app.get('/health', async (_req, res) => {
  try {
    const result = await getPool().request().query('SELECT COUNT(*) AS rows FROM dbo.Celebrities');
    res.json({ ok: true, database: process.env.DB_NAME || 'Celebrities', rows: result.recordset[0].rows });
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
    if (!Number.isInteger(id) || id <= 0) return res.status(400).json({ error: 'Invalid id' });

    const result = await getPool().request()
      .input('Id', sql.Int, id)
      .query('SELECT Id, FullName, Nationality, ReqPhotoPath FROM dbo.Celebrities WHERE Id = @Id');

    if (result.recordset.length === 0) return res.status(404).json({ error: 'Celebrity not found' });
    return res.json(result.recordset[0]);
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

app.post('/api/celebrities', async (req, res) => {
  try {
    const validation = validateCelebrity(req.body);
    if (validation) return res.status(400).json({ error: validation });

    const request = getPool().request();
    addCelebrityInputs(request, req.body);
    const result = await request.query(`
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
    if (!Number.isInteger(id) || id <= 0) return res.status(400).json({ error: 'Invalid id' });
    const validation = validateCelebrity(req.body);
    if (validation) return res.status(400).json({ error: validation });

    const request = getPool().request().input('Id', sql.Int, id);
    addCelebrityInputs(request, req.body);
    const result = await request.query(`
      UPDATE dbo.Celebrities
      SET FullName = @FullName,
          Nationality = @Nationality,
          ReqPhotoPath = @ReqPhotoPath
      OUTPUT INSERTED.Id, INSERTED.FullName, INSERTED.Nationality, INSERTED.ReqPhotoPath
      WHERE Id = @Id
    `);

    if (result.recordset.length === 0) return res.status(404).json({ error: 'Celebrity not found' });
    return res.json(result.recordset[0]);
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

app.delete('/api/celebrities/:id', async (req, res) => {
  try {
    const id = Number(req.params.id);
    if (!Number.isInteger(id) || id <= 0) return res.status(400).json({ error: 'Invalid id' });

    const result = await getPool().request()
      .input('Id', sql.Int, id)
      .query('DELETE FROM dbo.Celebrities OUTPUT DELETED.Id WHERE Id = @Id');

    if (result.recordset.length === 0) return res.status(404).json({ error: 'Celebrity not found' });
    return res.status(204).send();
  } catch (error) {
    return res.status(500).json({ error: error.message });
  }
});

async function start() {
  await ensureDatabase();
  const port = Number(process.env.APP_PORT || 8080);
  app.listen(port, () => {
    console.log(`TDWA07-01 started on port ${port}. DB: ${process.env.DB_HOST || 'nginx'}:${process.env.DB_PORT || '1433'}`);
  });
}

start().catch((error) => {
  console.error('Failed to start TDWA07-01:', error.message);
  process.exit(1);
});
