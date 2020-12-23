
#include <iostream>
#include <vector>
#include <string>
#include <any>
#include <typeinfo>

#include "../../common/util.hpp"

using namespace std;

//best practice, use nrvo(name return value optimize)
IntClass fun()
{
    IntClass a(4);
    cout << "fun" << endl;
    return move(a);
}

int main()
{
    IntClass b = fun();
    cout << b._ival << endl;
    cout << "end" << endl;


    return 0;
}