import 'package:flutter/material.dart';

main() => runApp(MaterialApp(
      home: Scaffold(
        body: Page(),
      ),
    ));

class Page extends StatelessWidget {
  final _fatherKey = GlobalKey<_FatherWidgetState>();
  @override
  Widget build(BuildContext context) {
    // return FatherWidget(key: _fatherKey,);
    return Column(
      children: [
        FatherWidget(key: _fatherKey),
        RaisedButton(onPressed: () {
          _fatherKey.currentState.changeValue('hi');
        }),
        // Text(_fatherKey.currentState.val),
      ],
    );
  }
}

class FatherWidget extends StatefulWidget {
  FatherWidget({Key key}) : super(key: key);

  @override
  _FatherWidgetState createState() => _FatherWidgetState();
}

class _FatherWidgetState extends State<FatherWidget> {
  String _val = "val";
  @override
  Widget build(BuildContext context) {
    return Text(_val);
  }

  void changeValue(final String value) {
    setState(() {
      _val = value;
    });
  }
}
