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
#include <algorithm>

using namespace std;

class VbgResource
{
public: 
    string m_name;

};

int main()
{
    std::vector<VbgResource> vec1{{"ooo"}, {"hi"}, {"hei"}};
    std::vector<VbgResource> vec2{{"hei"},{"hi"},  {"ooo"}};

    sort(vec2.begin(), vec2.end(), [&vec1](const VbgResource& left, const VbgResource& right){
        auto leftIter = std::find_if(vec1.begin(), vec1.end(), [left](const VbgResource& resource){ return resource.m_name == left.m_name;});
        auto rightIter = std::find_if(vec1.begin(), vec1.end(), [right](const VbgResource& resource){ return resource.m_name == right.m_name;});
        return leftIter < rightIter;
    });

    for(auto& ival : vec2)
    {
        cout << ival.m_name << endl;
    }

    map<int, int> a{ {1, 2}};
    a.erase(2);
    auto b = find(a.begin(), a.end(), 1);
    cout << a.size() << endl;
    a.erase(1);
    cout << a.size() << endl;


    return 0;
}