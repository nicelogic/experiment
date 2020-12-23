const ws = require('nodejs-websocket');
let pc1Socket = null;
let pc2Socket = null;
const server = ws.createServer(function(socket){
    pc1Socket = socket;
    socket.on('text', function(str) {
        console.log(str);
        if (pc2Socket != null) {
            pc2Socket.send(str);
        }
    });
}).listen(3000); 

const pc2Server = ws.createServer(function(socket){
    pc2Socket = socket;
    socket.on('text', function(str) {
        console.log(str);
        if (pc1Socket != null) {
            pc1Socket.send(str);
        }
    });
}).listen(3001); 