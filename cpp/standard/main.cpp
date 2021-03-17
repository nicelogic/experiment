#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <set>
#include <map>
#include <vector>
#include <typeinfo>

using namespace std;

vector<string> getStrings()
{
    return {
        "hi",
        "a"
    };
}

enum EMFoo
{
    EM_FOO
};

int main()
{
    for(auto& s : getStrings())
    {
        cout << s << endl;
    }

    cout << (typeid(EM_FOO).name()) << endl;

    return 0;
}