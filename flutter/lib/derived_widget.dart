// import 'package:flutter/material.dart';
// import 'package:provider/provider.dart';

// main() => runApp(MaterialApp(
//         home: ChangeNotifierProvider(
//       create: (context) => Data(),
//       child: PageStateEx(),
//     )));

// class Data extends ChangeNotifier {
//   int _val = 0;

//   int get val => _val;

//   increment() {
//     ++_val;
//     notifyListeners();
//   }
// }

// class Page extends StatelessWidget {
//   @override
//   Widget build(BuildContext context) {
//     return Column(
//       children: [
//         RaisedButton(
//           onPressed: () {
//             Provider.of<Data>(context, listen: false).increment();
//           },
//         ),
//         Consumer<Data>(
//           builder: (context, data, child) => Text('${data.val}'),
//         )
//       ],
//     );
//   }
// }

// class PageEx extends Page {
//   @override
//   Widget build(BuildContext context) {
//     final page = super.build(context);
//     return Column(
//       children: [page, Text("hello")],
//     );
//   }
// }

// class PageStateful extends StatefulWidget {
//   @override
//   State createState() => _PageState();
// }

// class _PageState extends State<PageStateful> {
//   @override
//   Widget build(BuildContext context) {
//     return Column(
//       children: [
//         RaisedButton(
//           onPressed: () {
//             Provider.of<Data>(context, listen: false).increment();
//           },
//         ),
//         Consumer<Data>(
//           builder: (context, data, child) => Text('${data.val}'),
//         )
//       ],
//     );
//   }
// }

// class PageStateEx extends PageStateful {
//   @override
//   _PageStateExState createState() => _PageStateExState();
// }

// class _PageStateExState extends _PageState {
//   @override
//   Widget build(BuildContext context) {
//     final page = super.build(context);
//     return Column(children: [
//       page,
//       Text("hello")
//     ],);
//   }
// }
