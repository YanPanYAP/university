const net = require('net');

let HOST = '127.0.0.1';
let PORT = 4000;

let x = process.argv[2]; 

if(x == undefined || isNaN(Number(x))){
    console.error('Ошибка: укажите число Х в параметрах');
    process.exit(1);
}

x = parseInt(x, 10);

let client = new net.Socket();
let buf = new Buffer.alloc(4);
let timerID = null;
client.connect(PORT, HOST, () => {
    console.log(`Client CONNECTED: ${client.remoteAddress} ${client.remotePort}`);
    timerID = setInterval(() => {
        client.write((buf.writeInt32LE(x, 0), buf))
    }, 1000);
    setTimeout(() => {
        clearInterval(timerID);
        client.end();
    }, 20000);
});

client.on('data', (data) => {
    console.log(`Client DATA: ${data.readInt32LE()}`);
    
});

client.on('close', () => {
    console.log('Client CLOSED');
});

client.on('error', () => {
    console.log('Client ERROR');
});