
#include <iostream>

using namespace std;

class Test {
public:
  int _ival = 3;
};

class Derived : private Test {
public:
  void fun() { cout << _ival << endl; }
};

int main() {
  Derived d;
  d.fun();

  return 0;
}