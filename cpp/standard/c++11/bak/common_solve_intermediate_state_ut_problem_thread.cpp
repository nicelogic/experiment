
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

int g_ival{0};
map<unique_ptr<promise<bool>>, pair<promise<bool>, bool>> mapPromise;

void onGrupUpdate()
{
    ++g_ival;
    cout << "[bussiness-thread]onGroupUpdate, set value: " << g_ival << endl;
    for (auto &promisePair : mapPromise)
    {
        cout << "[bussiness-thread]test future whether valid?" << endl;
        if(!promisePair.second.second)
        {
            cout << "[bussiness-thread]is valid" << endl;
            promisePair.first->set_value(true);
            cout << "[bussiness-thread]promise set value" << endl;
            cout << "[bussiness-thread]need to wait" << endl;
            promisePair.second.first.get_future().wait();
            promisePair.second.second = true;
            cout << "[bussiness-thread]continue excution" << endl;
            break;
        }
        else
        {
            cout << "[bussiness-thread]not valid" << endl;
        }
        
    }
}

int main()
{
    mapPromise[make_unique<promise<bool>>()] = make_pair(promise<bool>(), false);
    // mapPromise[make_unique<promise<bool>>()] = pair<promise<bool>, bool>(promise<bool>(), false);

    thread([&]() {
        thread([&](){
            this_thread::sleep_for(seconds(1));
            onGrupUpdate();
        }).join();

        thread([&]() {
            onGrupUpdate();
        }).detach();
    }).detach();

    for (auto &promisePair : mapPromise)
    {
        cout << "[ut-thread]wait for value" << endl;
        promisePair.first->get_future().wait();
        cout << "[ut-thread]test state:" << g_ival << endl;
        promisePair.second.first.set_value(true);
        cout << "[ut-thread]set value for business thread continue" << endl;
    }
    cout<< "[ut-thread]test end" << endl;

    return 0;
}