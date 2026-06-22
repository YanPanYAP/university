
const redis = require('redis');

async function testHSet(client, count) {
    console.time('HSET 10000');
    for (let i = 1; i <= count; i++) {
        await client.hSet(`hash:${i}`, {
        id: String(i),
        val: `val-${i}`
    });
    }
    console.timeEnd('HSET 10000');
}

async function testHGet(client, count) {
    console.time('HGET 10000');
    for (let i = 1; i <= count; i++) {
        await client.hGetAll(`hash:${i}`);
    }
    console.timeEnd('HGET 10000');
}

async function main() {
    const client = redis.createClient();
    await client.connect();

    const COUNT = 10000;

    await testHSet(client, COUNT);
    await testHGet(client, COUNT);

    await client.quit();
}

main();