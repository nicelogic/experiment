
#include <iostream>
#include <string>
#include <any>

#include "json.hpp"

using namespace std;
using Json = nlohmann::json;

int main()
{
    auto json1 = Json::parse(R"({"a": "b"})");
    const auto &b = json1["a"].get<string>();
    std::any jval = std::move(b);
    cout << b << endl;
    cout << jval.type().name() << endl;

    auto json2 = json1;
    cout << json2 << endl;
    return 0;
}