
#include <iostream>
#include <map>

using namespace std;

class Base
{
public:
    Base(int ival):_ival(ival){}
    int _ival;
};

int main()
{
    map<int, Base> imap;
    cout << imap[3]._ival << endl;
    return 0;
}