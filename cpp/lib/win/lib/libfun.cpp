

#include <iostream>

using namespace std;

int __declspec(dllexport) Fun()
{
	int i = 3;
	cout << "hi:" << i << endl;
	return i;
}
