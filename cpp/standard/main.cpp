#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>

using namespace std;

bool foo()
{
    return true;
}
int main()
{
    auto a = []() -> bool { return true; };
    function<void()> fun = foo;
    function<void()> fun2 = a;

    return 0;
}