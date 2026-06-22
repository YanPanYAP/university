const fs = require('fs');
const path = require('path');
const { connectWithRetry, getPool } = require('../src/db');

async function main() {
  const inputPath = process.argv[2];
  if (!inputPath) {
    console.error('Usage: node ./scripts/run-sql.js <path-to-sql-file>');
    process.exit(1);
  }

  const sqlFilePath = path.resolve(process.cwd(), inputPath);
  if (!fs.existsSync(sqlFilePath)) {
    console.error(`SQL file not found: ${sqlFilePath}`);
    process.exit(1);
  }

  const script = fs.readFileSync(sqlFilePath, 'utf8');
  await connectWithRetry();

  const batches = script
    .split(/^\s*GO\s*$/gim)
    .map((s) => s.trim())
    .filter(Boolean);

  for (const [index, batch] of batches.entries()) {
    const result = await getPool().request().batch(batch);
    if (result.recordset && result.recordset.length > 0) {
      console.log(`Batch ${index + 1} result:`);
      console.log(JSON.stringify(result.recordset, null, 2));
    }
  }

  console.log(`Executed ${batches.length} SQL batch(es) from ${inputPath}`);
  process.exit(0);
}

main().catch((error) => {
  console.error('SQL execution failed:', error.message);
  process.exit(1);
});
