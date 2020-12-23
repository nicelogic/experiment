

#include <iostream>

using namespace std;

static int gs_int;
class CA
{
public:

    int m_ival{};
    char int8array[20]{};
};

int main()
{
    CA a;
    cout << a.m_ival << endl;
    cout << a.int8array << endl;
    cout << gs_int << endl;
    return 0;
}

/*
result: compile ok
*/