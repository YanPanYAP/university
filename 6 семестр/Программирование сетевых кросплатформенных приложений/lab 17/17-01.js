const redis = require('redis');

async function testConnection() {
    const client = redis.createClient({
        socket: {
            host: '127.0.0.1',
            port: 6379
        }
    });

    client.on('error', (err) => {
        console.error('Redis error:', err);
    });

    await client.connect();

    console.log('Connected to Redis');

    const response = await client.ping();
    console.log('PING response:', response);

    await client.quit();
}

async function main() {
    await testConnection();
}

main();