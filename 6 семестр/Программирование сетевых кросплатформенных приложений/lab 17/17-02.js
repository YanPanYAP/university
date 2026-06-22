const redis = require('redis');

async function testSet(client, count) {
    console.time('SET 10000');
    for (let i = 1; i <= count; i++) {
        await client.set(String(i), `set${i}`);
    }
    console.timeEnd('SET 10000');
}

async function testGet(client, count) {
    console.time('GET 10000');
    for (let i = 1; i <= count; i++) {
        await client.get(String(i));
    }
    console.timeEnd('GET 10000');
}

async function testDel(client, count) {
    console.time('DEL 10000');
    for (let i = 1; i <= count; i++) {
        await client.del(String(i));
    }
    console.timeEnd('DEL 10000');
}

async function main() {
    const client = redis.createClient();
    await client.connect();

    const COUNT = 10000;

    await testSet(client, COUNT);
    await testGet(client, COUNT);
    await testDel(client, COUNT);

    await client.quit();
}

main();