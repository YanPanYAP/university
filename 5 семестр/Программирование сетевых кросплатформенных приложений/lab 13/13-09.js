const udp = require('dgram');
const PORT = 3000;

let server = udp.createSocket('udp4');

server.on('error', (err) => {
    console.log(`Ошибка: ${err}`);
    server.close();
});

server.on('message', (msg, info) => {
    console.log(`Server: от клиента получено: ${msg.toString()}`);
    console.log('Server: получено %d байтов от %s:%d\n', msg.length, info.address, info.port);

    let echoMes = 'ECHO: ' + msg;
    server.send(echoMes, info.port, info.address, (err) => {
        if(err) server.close();
        else console.log('Server: данные отправлены клиенту');
    });
});

server.on('listening', () => {
    console.log(`Server: слушает порт ${server.address().port}`);
    console.log(`Server: ip сервера ${server.address().address}`);
    console.log(`Server: семейство(IP4/IP6) ${server.address().family}`);
});

server.on('close', () => {
    console.log('Server: сокет закрыт');
});

server.bind(PORT);