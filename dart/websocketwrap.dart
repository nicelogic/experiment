// import 'dart:html' show MessageEvent, WebSocket;
// import 'dart:async';

// class WebSocketWrap {
//   WebSocket _webSocket;
//   bool _isFirstTimeConnected = true;
//   bool _isConnected;

//   WebSocketWrap(final String addr) {
//     initWebSocket(addr);
//   }

//   bool isConnected() {
//     return _isConnected;
//   }

//   void initWebSocket(final String addr, [int retrySeconds = 3]) {
//     var reconnectScheduled = true;

//     print("Connecting to websocket");
//     _webSocket = WebSocket(addr);

//     void scheduleReconnect() {
//       _isConnected = false;
//       if (reconnectScheduled) {
//         Timer(Duration(milliseconds: 1000 * retrySeconds),
//             () => initWebSocket(addr, retrySeconds));
//       }
//       reconnectScheduled = false;
//     }

//     alwaysPing() async {
//       if (_isConnected) {
//         _webSocket.send('ping');
//         print('ping');
//       }
//       Future.delayed(Duration(seconds: 10), alwaysPing);
//     }

//     _webSocket.onOpen.listen((e) {
//       print('Connected');
//       _isConnected = true;
//       if (_isFirstTimeConnected) {
//         _isFirstTimeConnected = false;
//         alwaysPing();
//       }
//     });

//     _webSocket.onClose.listen((e) {
//       print('Websocket closed, retrying in $retrySeconds seconds');
//       scheduleReconnect();
//     });

//     _webSocket.onError.listen((e) {
//       print("Error connecting to ws");
//       scheduleReconnect();
//     });

//     _webSocket.onMessage.listen((MessageEvent e) {
//       print('Received message: ${e.data}');
//     });
//   }
// }
