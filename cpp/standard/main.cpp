#include <algorithm>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <vector>

using namespace std;

int main()
{
    ifstream ifs("log.txt");
    if (!ifs.is_open())
    {
        cerr << "fs error" << endl;
        return 0;
    }

    ostringstream oss;
    oss << ifs.rdbuf();

    set<string> createEncoders;
    {
        std::istringstream iss_test(oss.str());
        string s(1000, '\0');
        while (iss_test.getline(&s[0], 1000))
        {
            if (s.find("Create compression session") != std::string::npos)
            {
                s = s.substr(s.find_first_of(","));
                s = s.substr(s.find_first_of("111"));
                s = s.substr(s.find_first_of(":") + 2);
                // cout << s.c_str() << "|" << endl;
                createEncoders.insert(s);
            }
            s.clear();
            s.resize(1000);
        }
        cout << "create encoders size: " << createEncoders.size() << endl;
        for(auto s : createEncoders){
            cout << s << endl;
        }
    }

    set<string> releaseEncoders;
    {
        std::istringstream iss_test2(oss.str());
        string s2(500, '\0');
        while (iss_test2.getline(&s2[0], 500))
        {
            if (s2.find("Release compression session") != std::string::npos)
            {
                s2 = s2.substr(s2.find_first_of(","));
                s2 = s2.substr(s2.find_first_of("222"));
                s2 = s2.substr(s2.find_first_of(":") + 13);
                releaseEncoders.insert(s2);
            }
            s2.clear();
            s2.resize(1000);
        }
        cout << "release encoders size: " << releaseEncoders.size() << endl;
        for(auto s : releaseEncoders){
            cout << s << endl;
        }
    }

    vector<string> diffs;
    std::set_difference(createEncoders.begin(), createEncoders.end(), releaseEncoders.begin(), releaseEncoders.end(),
                        std::back_inserter(diffs));
    cout << diffs.size() << endl;
    

        ifs.close();
    return 0;
}