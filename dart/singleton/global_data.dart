// import 'package:app/logic_util/websocketwrap.dart';
// import 'package:shared_preferences/shared_preferences.dart';

class AppData {
  bool _isHasLogin = false;
  // final WebSocketWrap _webSocketWrap = WebSocketWrap('wss://echo.websocket.org');

  static final AppData _singleton = AppData._internal();
  factory AppData() {
    return _singleton;
  }
  AppData._internal();

  void setHasLogin(bool bHasLogin) {
    _isHasLogin = bHasLogin;
  }

  bool hasLogin() {
    return _isHasLogin;
  }
}

initAppDataFromPersistenceData() async {
  // final preference = await SharedPreferences.getInstance();
  // final bHasLogin = preference.getBool("hasLogin") ?? false;
  AppData().setHasLogin(true);
}
