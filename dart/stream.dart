main() {
  Stream.periodic(Duration(seconds: 1), (x) => print("$x"))
      .take(10)
      .listen((event) {
    // print(event);
  });

//will nothing todo 
  Stream.periodic(Duration(seconds: 1), (x) => print("$x")).take(10);

  Stream.periodic(Duration(seconds: 1) 
  ).take(10).listen((event) {
    print('event');
  });
  return 0;
}
