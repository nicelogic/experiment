
#include <string>
#include <iostream>

using namespace std;
class base
{
	public:
		virtual void fun()
		{
			base::fun2();
		}
		virtual void fun2()
		{
			cout << "fun2" << endl;
		}
};

class derive : public base
{
	public:
		void fun2()
		{
			cout << "fun2 derived" << endl;
		}
};


int main()
{
	derive d;
	base *p = &d;
	p->fun();

	return 0;
}