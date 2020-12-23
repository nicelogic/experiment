#include <chrono>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>

using namespace std;

class A{
    public:
    virtual ~A(){};
};
class B : public A{};
class Cfoo : public B{};

void fun(const string &str)
{
    cout << (str + "hi") << endl;
}

int main()
{

    fun(string("dafa"));
    Cfoo c;
    A* a = new  Cfoo();
    cout << typeid(*a).name() << endl;
    type_info

    return 0;
}