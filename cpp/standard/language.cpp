
#include <iostream>

using namespace std;

//test return value
class A
{
public:
    const string GetStr() const
    {
        return _str;
    }

    string _str;
};

int main()
{
    A a;
    string ival(a.GetStr());
    cout << ival << endl;

    //uintptr_t
    void *p;

    cout << (uintptr_t)p << endl;
    cout << p << endl;

    return 0;
}