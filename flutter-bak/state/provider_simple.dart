import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

main() => runApp(MaterialApp(
        home: ChangeNotifierProvider(
      create: (context) => Data(),
      child: Page(),
    )));

class Data extends ChangeNotifier {
  int _val = 0;

  int get val => _val;

  increment() {
    ++_val;
    notifyListeners();
  }
}

class Page extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        RaisedButton(
          onPressed: () {
            Provider.of<Data>(context, listen: false).increment();
          },
        ),
        Consumer<Data>(
          builder: (context, data, child) =>
              Text('consumer usage: data: ${data.val}'),
        ),
        Text(
            'provider usage: data: ${Provider.of<Data>(context, listen: false).val}')
      ],
    );
  }
}
