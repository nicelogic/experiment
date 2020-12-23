
#include <iostream>
#include <string>

using namespace std;

template<typename... Args>
bool all(Args... args) { return (... && args); }
 
bool b = all(true, true, true, false);

template<typename... Args>
void Printer(Args&&... args)
{
    (cout << ...  << args << " | ") << endl;
}

int main()
{
    cout << b << endl;
    Printer(1, 2, "hi", "v", 1.34, string("hi"));

    return 0;
}