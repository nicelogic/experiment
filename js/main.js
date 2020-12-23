const path = require('path');

const os = require('os');
console.log(os.platform);
console.log(process.cwd());
const SOCKET_PATH_WIN = path.join('\\\\?\\pipe', process.cwd(), 'mediasoup-demo');

console.log(SOCKET_PATH_WIN);