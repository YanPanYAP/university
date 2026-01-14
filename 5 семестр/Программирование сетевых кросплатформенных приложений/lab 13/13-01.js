const net = require('net');

let HOST = '0.0.0.0';
let PORT = 4000;

net.createServer((sock) => {
    console.log(`Server connected: ${sock.remoteAddress} : ${sock.remotePort}`);

    sock.on('data', (data) => {
        console.log(`Server DATA: ${sock.remoteAddress} : ${data}`);
        sock.write(`ECHO: ${data}`);
    });

    sock.on('close', (data) => {
        console.log(`Server CLOSED: ${sock.remoteAddress} ${sock.remotePort}`);
    });
}).listen(PORT, HOST);

console.log(`TCP-server ${HOST} : ${PORT}`);