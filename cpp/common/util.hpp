
#include <iostream>

using namespace std;

class IntClass
{
public:
    IntClass(int ival = 0) : _ival(ival)
    {
        cout << "construct IntClass, ival: " << _ival
             << "(" << this << ")" << endl;
    }

    IntClass(const IntClass& other)
    {
        cout << "copy construct" << endl;
        _ival = other._ival;
    }

    ~IntClass()
    {
        _ival = -1;
        cout << "destruct IntClass, (" << this << ")" << endl;
    }

    int _ival{1};
};