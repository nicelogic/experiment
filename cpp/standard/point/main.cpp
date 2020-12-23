
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <memory>
#include <algorithm>

#include "../../common/util.hpp"

using namespace std;

set<unique_ptr<IntClass>> paSet;

//this is the best practice, because nrvo
IntClass getThenRemoveA(const uintptr_t a)
{
    //don't move A c to the A c = *val in the for loop; because use the nrvo
    //that will decrease once construct
    IntClass c;
    for (const auto &val : paSet)
    {
        if (val.get() == reinterpret_cast<unique_ptr<IntClass>::pointer>(a))
        {
            c = *val;
            paSet.erase(val);
            return c;
        }
    }
    return c;
}

IntClass& get(const uintptr_t a)
{
    //don't move A c to the A c = *val in the for loop; because use the nrvo
    //that will decrease once construct
    for (const auto &val : paSet)
    {
        if (val.get() == reinterpret_cast<unique_ptr<IntClass>::pointer>(a))
        {
            return *val;
        }
    }
    static IntClass b;
    return b;
}

int main()
{
    {
        const uintptr_t a = reinterpret_cast<uintptr_t>(paSet.emplace(new IntClass(9)).first->get());
        get(a)._ival = 10;
        cout << get(a)._ival << endl;
    }
    {
        // const uintptr_t a = reinterpret_cast<uintptr_t>(paSet.emplace(new IntClass(9)).first->get());
        // getThenRemoveA(a);
    }
    {
        //right
        // unique_ptr<IntClass> ps(new IntClass(3));
        // unique_ptr<IntClass> ps2;
        // ps2 = move(ps);
    }

    return 0;
}