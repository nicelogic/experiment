import 'package:bloc/bloc.dart';
import 'package:test/test.dart';
import 'package:bloc_test/bloc_test.dart';

enum CounterEvent { increment, decrement }

class CounterBloc extends Bloc<CounterEvent, int> {
  CounterBloc() : super(0);

  @override
  Stream<int> mapEventToState(CounterEvent event) async* {
    switch (event) {
      case CounterEvent.decrement:
        yield state - 1;
        break;
      case CounterEvent.increment:
        yield state + 1;
        break;
    }
  }
}

main() {
  group('CounterBloc', () {
    CounterBloc counterBloc;

    setUp(() {
      counterBloc = CounterBloc();
    });

    test('initial state is 0', () {
      expect(counterBloc.state, 0);
    });

    blocTest(
      'emits [1] when CounterEvent.increment is added',
      build: () => counterBloc,
      act: (bloc) => bloc.add(CounterEvent.increment),
      expect: [2],
    );

    blocTest(
      'emits [-1] when CounterEvent.decrement is added',
      build: () => counterBloc,
      act: (bloc) => bloc.add(CounterEvent.decrement),
      expect: [-1],
    );
  });
}
