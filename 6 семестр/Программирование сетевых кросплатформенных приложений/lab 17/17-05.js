
const redis = require('redis');

async function startSubscriber(subscriber) {
    await subscriber.subscribe('channel1', (message) => {
        console.log('Received message:', message);
    });
}

async function publishMessage(publisher) {
    await publisher.publish('channel1', 'Hello Redis!');
}

async function main() {
    const subscriber = redis.createClient();
    const publisher = redis.createClient();

    await subscriber.connect();
    await publisher.connect();

    await startSubscriber(subscriber);

    setTimeout(async () => {
        await publishMessage(publisher);
    }, 1000);
}

main();