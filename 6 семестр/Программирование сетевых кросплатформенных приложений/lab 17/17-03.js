
const redis = require('redis');

async function testIncr(client, count) {
    console.time('INCR 10000');
    for (let i = 0; i < count; i++) {
        await client.incr('incr');
    }
    console.timeEnd('INCR 10000');
}

async function testDecr(client, count) {
    console.time('DECR 10000');
    for (let i = 0; i < count; i++) {
        await client.decr('incr');
    }
    console.timeEnd('DECR 10000');
}

async function main() {
    const client = redis.createClient();
    await client.connect();

    const COUNT = 10000;

    await client.set('incr', 0);

    await testIncr(client, COUNT);
    await testDecr(client, COUNT);

    await client.quit();
}

main();