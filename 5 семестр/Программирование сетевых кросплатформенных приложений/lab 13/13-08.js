const net = require('net');

let HOST = '127.0.0.1';
let PORT = process.argv[2]? process.argv[2] : 40000;

let client = new net.Socket();
let buf = new Buffer.alloc(4);
let timerID = null;
client.connect(PORT, HOST, () => {
    console.log(`Client CONNECTED: ${client.remoteAddress} ${client.remotePort}`);
    let k = 0;
    timerID = setInterval(() => {
        client.write((buf.writeInt32LE(k++, 0), buf))
    }, 1000);
    setTimeout(() => {
        clearInterval(timerID);
        client.end();
    }, 20000);
});

client.on('data', (data) => {
    console.log(`Client DATA: ${data}`);
    
});

client.on('close', () => {
    console.log('Client CLOSED');
});

client.on('error', () => {
    console.log('Client ERROR');
});