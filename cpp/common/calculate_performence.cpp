
#include <boost/thread.hpp>

{
	auto startTest = boost::chrono::steady_clock::now();
	///////////////////////////////
	//....
	///////////////////////////////
	auto endTest = boost::chrono::steady_clock::now() - startTest;
	std::cout << boost::chrono::duration_cast<boost::chrono::microseconds>(endTest).count() << endl;
}
