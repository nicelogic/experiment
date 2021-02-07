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

using namespace std;

int main()
{
    set<bool> bSets;

    bSets.emplace(true);
    cout << bSets.size() << endl;

    return 0;
}