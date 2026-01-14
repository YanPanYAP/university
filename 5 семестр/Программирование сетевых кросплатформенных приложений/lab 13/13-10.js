const buffer = require('buffer');
const udp = require('dgram');
const client = udp.createSocket('udp4');
const PORT = 3000;

client.on('message', (msg, info) => {
    console.log(`Client: от сервера получено: ${msg.toString()}`);
    console.log('CLient: получено %d байтов от %s:%d\n', msg.length, info.address, info.port);
});

let data = Buffer.from('Client: Hello world');
client.send(data, PORT, 'localhost', (err) => {
    if(err) client.close();
    else console.log('Client: сообщение отправлено серверу');
});
