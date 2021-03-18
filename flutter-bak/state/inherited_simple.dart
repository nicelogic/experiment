import 'package:flutter/material.dart';

main() => runApp(MaterialApp(
    home: Page(
        data: Data(val: 'hi'),
        child: Column(
          children: [
            TextWrap(),
          ],
        ))));

class Data {
  Data({this.val});
  String val = 'hi';
}

class Page extends InheritedWidget {
  final Data data;

  Page({Key key, @required child, this.data}) : super(key: key, child: child);

  @override
  bool updateShouldNotify(Page old) => old.data != data;

  static Page of(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<Page>();
  }
}

class TextWrap extends StatefulWidget {
  @override
  _TextWrapState createState() => _TextWrapState();
}

class _TextWrapState extends State<TextWrap> {
  @override
  Widget build(BuildContext context) {
    String val = Page.of(context).data.val;
    return Text('can auto update by big brother $val');
  }
}
