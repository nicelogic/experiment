// import 'package:bloc/bloc.dart';
// import 'package:flutter_bloc/flutter_bloc.dart';
// import 'package:flutter/material.dart';

// main() {
//   runApp(MaterialApp(
//     home: BlocProvider<AppBloc>(create: (context) => AppBloc(), child: Page()),
//   ));
// }

// class Page extends StatelessWidget {
//   @override
//   Widget build(BuildContext context) {
//     return BlocBuilder<AppBloc, AppData>(builder: (context, appData) {
//       return Column(
//         children: [
//           Text('is connected: ${appData.bConnected}'),
//           RaisedButton(
//             onPressed: () {
//               BlocProvider.of<AppBloc>(context).add(EMAppEvent.CONNECTED);
//             },
//           )
//         ],
//       );
//     });
//   }
// }

// enum EMAppEvent { CONNECTED, DISCONNECTED }

// class AppData {
//   bool bConnected = false;

//   setConnected({bool bConnectedServer}) {
//     bConnected = bConnectedServer;
//   }

//   AppData({this.bConnected});

//   AppData copyWith({bool bConnected}) {
//     return AppData(
//       bConnected: bConnected??this.bConnected,
//     );
//   }
// }

// class AppBloc extends Bloc<EMAppEvent, AppData> {
//   @override
//   AppData get initialState => AppData(bConnected: false);

//   @override
//   void onTransition(Transition<EMAppEvent, AppData> transition) {
//     print(transition);
//     super.onTransition(transition);
//   }

//   @override
//   Stream<AppData> mapEventToState(EMAppEvent event) async* {
//     switch (event) {
//       case EMAppEvent.CONNECTED:
//         print('recv conn');
//         yield state.copyWith(bConnected: true);
//         break;
//       case EMAppEvent.DISCONNECTED:
//         state.setConnected(bConnectedServer: false);
//         yield state;
//         break;
//     }
//   }
// }
