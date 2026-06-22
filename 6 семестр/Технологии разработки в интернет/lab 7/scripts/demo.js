#!/usr/bin/env node
'use strict';

const BASE_URL = process.env.APP_URL || 'http://localhost:3007';

async function request(method, urlPath, body) {
  const options = { method, headers: {} };
  if (body) {
    options.headers['Content-Type'] = 'application/json';
    options.body = JSON.stringify(body);
  }
  const response = await fetch(`${BASE_URL}${urlPath}`, options);
  const text = await response.text();
  let data = null;
  if (text) {
    try {
      data = JSON.parse(text);
    } catch {
      data = text;
    }
  }
  return { method, path: urlPath, status: response.status, data };
}

async function waitForApp() {
  let lastError;
  for (let attempt = 1; attempt <= 60; attempt += 1) {
    try {
      const response = await request('GET', '/health');
      if (response.status === 200) return response;
      lastError = new Error(`HTTP ${response.status}`);
    } catch (error) {
      lastError = error;
    }
    await new Promise((resolve) => setTimeout(resolve, 2000));
  }
  throw lastError;
}

async function main() {
  const results = [];
  const health = await waitForApp();
  results.push({ operation: 'health', ...health });

  const created = await request('POST', '/api/celebrities', {
    FullName: 'Milla Jovovich',
    Nationality: 'UA',
    ReqPhotoPath: '/photos/milla.jpg'
  });
  results.push({ operation: 'create', ...created });

  const id = created.data.id ?? created.data.Id;
  if (!id) {
    throw new Error(`Create response does not contain id: ${JSON.stringify(created.data)}`);
  }
  results.push({ operation: 'read all', ...(await request('GET', '/api/celebrities')) });
  results.push({ operation: 'read one', ...(await request('GET', `/api/celebrities/${id}`)) });
  results.push({
    operation: 'update',
    ...(await request('PUT', `/api/celebrities/${id}`, {
      FullName: 'Milla Jovovich Updated',
      Nationality: 'UA',
      ReqPhotoPath: '/photos/milla-updated.jpg'
    }))
  });
  results.push({ operation: 'delete', ...(await request('DELETE', `/api/celebrities/${id}`)) });

  console.log('lab 7');
  for (const row of results) {
    console.log(`${row.operation}: ${row.method} ${row.path} -> ${row.status}`);
  }
  console.log(`created id: ${id}`);
  console.log(`health: ${JSON.stringify(health.data)}`);
}

main().catch((error) => {
  console.error('Demo failed:', error.message);
  process.exit(1);
});
