
#include <iostream>
#include <map>
#include <list>
#include <set>
#include <memory>
#include <algorithm>

using namespace std;

class A
{
public:
    A() { cout << "construct a, _ival:" << _ival << endl; }
    ~A() { cout << "destroyed a" << endl; }
    int _ival{3};
};
set<unique_ptr<A>> paSet;

unique_ptr<A> removeA(const uintptr_t a)
{
    paSet.erase(unique_ptr<A>((unique_ptr<A>::pointer)a));
}


int main()
{
    A b;
    // const uintptr_t a = reinterpret_cast<uintptr_t>(paSet.emplace(new A(b)).first->get());
    // cout << a << endl;
    // cout << paSet.size() << endl;
    // paSet.erase(unique_ptr<A>((unique_ptr<A>::pointer)a));
    // cout << paSet.size() << endl;

    const uintptr_t c = reinterpret_cast<uintptr_t>(paSet.emplace(new A(b)).first->get());
    cout << paSet.size() << endl;
    auto iter = paSet.find(unique_ptr<A>((unique_ptr<A>::pointer)c));
    cout << iter->get()->_ival << endl;


    return 0;
}