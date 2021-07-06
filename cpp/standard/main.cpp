#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>

using namespace std;

template <typename SearchFunType>
void debounceTask(const SearchFunType fun)
{
    //cout << type_info(fun).name() << endl;
    thread([&]()
           { fun(); })
        .detach();
}
class Foo : public std::enable_shared_from_this<Foo>
{
public:
    void _search()
    {
        cout << "hi" << endl;
    }
    void search()
    {
        const auto fun = std::bind(&Foo::_search, shared_from_this());
        debounceTask(fun);
    }
};

int main()
{
    string name = "abc-123";
    cout << name.substr(name.find("-")) << endl;



    return 0;
}