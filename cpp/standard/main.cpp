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
#include <typeinfo>

using namespace std;

int main()
{
    std::chrono::time_point<std::chrono::steady_clock> aTime = std::chrono::steady_clock::now();
    this_thread::sleep_for(chrono::seconds(1));
    auto bTime =std::chrono::steady_clock::now(); 
     std::chrono::duration<double, std::milli> diff = bTime - aTime;
     cout << diff.count() << endl;

}