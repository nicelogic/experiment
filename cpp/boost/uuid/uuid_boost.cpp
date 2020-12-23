
#include <iostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    for (auto i = 0; i != stoi(argv[1]); ++i)
    {
        boost::uuids::uuid aUuid = boost::uuids::random_generator_mt19937()();
        boost::uuids::random_generator()();
        cout << boost::uuids::to_string(aUuid) << endl;
        
    }
    return 0;
}