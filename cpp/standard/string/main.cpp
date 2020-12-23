
#include <iostream>
#include <cstring>
#include <stdio.h>

using namespace std;

int main()
{
    unsigned int dwConfE164 = strtoul("1234865", 0, 10);
    cout << dwConfE164 << endl;
    cout << "hi" << endl;
    char achConfE164[8] = { 0 };
	sprintf(achConfE164, "%.06d", dwConfE164);
    cout << achConfE164 << endl;

    return 0;
}