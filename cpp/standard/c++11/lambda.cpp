
#include <functional>
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str("hi");
    auto fun(
        [str]() mutable {
            str = "hello";
            cout << str << endl;
        });
    fun();

    return 0;
}