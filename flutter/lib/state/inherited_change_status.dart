import 'package:flutter/material.dart';

main() => runApp(MaterialApp(
        home: MyInheritedWidget(
      child: Column(
        children: <Widget>[
          new WidgetA(),
          new WidgetB(),
        ],
      ),
    )));

class MyInherited extends InheritedWidget {
  MyInherited({
    Key key,
    @required Widget child,
    @required this.data,
  }) : super(key: key, child: child);

  final MyInheritedWidgetState data;

  @override
  bool updateShouldNotify(MyInherited oldWidget) {
    return true;
  }
}

class MyInheritedWidget extends StatefulWidget {
  MyInheritedWidget({
    Key key,
    this.child,
  }) : super(key: key);

  final Widget child;

  @override
  MyInheritedWidgetState createState() => new MyInheritedWidgetState();

  static MyInheritedWidgetState of(BuildContext context) {
    return context.dependOnInheritedWidgetOfExactType<MyInherited>().data;
  }
}

class MyInheritedWidgetState extends State<MyInheritedWidget> {
  /// List of Items
  List<String> _items = <String>[];

  /// Getter (number of items)
  int get itemsCount => _items.length;

  /// Helper method to add an Item
  void addItem(String reference) {
    setState(() {
      _items.add("aaa");
    });
  }

  @override
  Widget build(BuildContext context) {
    return new MyInherited(
      data: this,
      child: widget.child,
    );
  }
}

class WidgetA extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final state = MyInheritedWidget.of(context);
    return new Container(
      child: new RaisedButton(
        child: new Text('Add Item'),
        onPressed: () {
          state.addItem('new item');
        },
      ),
    );
  }
}

class WidgetB extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final state = MyInheritedWidget.of(context);
    return new Text('${state.itemsCount}');
  }
}
