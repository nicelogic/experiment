
#include <chrono>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

class ThreadsafePromiseContainer
{
public:
    size_t pushBack(promise<void> &promise)
    {
        lock_guard<mutex> lock(m_mutex);
        vecPromise.push_back(move(promise));
        return vecPromise.size();
    }

    void setValue(const size_t index)
    {
        lock_guard<mutex> lock(m_mutex);
        vecPromise[index].set_value();
    }

    size_t size()
    {
        lock_guard<mutex> lock(m_mutex);
        return vecPromise.size();
    }

    void waitAllPromisesFinished()
    {
        //这段代码，临时写成，待优化,且可能有线程安全问题
        bool isChanged = false;
        auto validPromiseIndex = 0; //because promise.get_future().valie() == false is undefined behavior
        do
        {
            isChanged = false;
            for (; validPromiseIndex != size(); ++validPromiseIndex)
            {
                const auto currentPrimiseCount = size();
                vecPromise[validPromiseIndex].get_future().wait();
                if (size() != currentPrimiseCount)
                {
                    isChanged = true;
                    ++validPromiseIndex;
                    break;
                }
            }
        } while (isChanged);
    }

private:
    mutex m_mutex;
    vector<promise<void>> vecPromise;
};

void dispatchThread(
    function<void(size_t index, function<void(size_t)>)> fun, //fun中去set value
    function<void(size_t)> completion = nullptr,
    ThreadsafePromiseContainer *p = nullptr)
{
    size_t size = 0;
    if (p != nullptr)
    {
        promise<void> aPromise;
        size = p->pushBack(aPromise);
    }
    cout << "size: " << size << endl;
    thread(fun, size - 1, completion).detach();
}

void doBusiness(function<void(size_t)> completion = nullptr,
                ThreadsafePromiseContainer *p = nullptr)
{
    dispatchThread(
        [](size_t index, function<void(size_t)> completion)
        {
            cout << "do something, index:" << index << endl;
            completion(index);
        },
        completion,
        p
    );
}

int main()
{
    ThreadsafePromiseContainer promises;
    auto completion = [&promises](size_t index) {
        cout << "thread<" << index << "> has completed, set value to mark" << endl;
        promises.setValue(index);
    };
    doBusiness(completion, &promises);
    promises.waitAllPromisesFinished();

    cout << "all sub task has finished" << endl;
    return 0;
}