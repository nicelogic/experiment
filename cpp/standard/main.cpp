#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <sstream>
#include <thread>
#include <typeinfo>
#include <set>
#include <map>
#include <vector>

using namespace std;

int main()
{
    cout << "{\"timestamp\":1613637988669,\"objects\":[[{\"created_at\":1613637988648,\"creator_id\":3671465987,\"version\":6590422032765928,\"model_size\":0,\"is_new\":true,\"item_ids\":[],\"post_ids\":[],\"group_id\":7701864450,\"text\":\"11\",\"_id\":4370104324,\"is_team_mention\":false,\"is_admin_mention\":false,\"source\":\"mobile\",\"unique_id\":\"3693869146980124\",\"function_id\":\"post\",\"company_id\":392855553,\"items\":[],\"history\":[{\"group_id\":7701864450,\"text\":\"11\",\"is_team_mention\":false,\"is_admin_mention\":false,\"creator_id\":3671465987,\"is_new\":true,\"source\":\"mobile\",\"new_version\":6590422032765928,\"unique_id\":\"3693869146980124\",\"function_id\":\"post\",\"company_id\":392855553,\"items\":[],\"created_at\":1613637988648}],\"modified_at\":1613637988648,\"deactivated\":false}]],\"message_id\":\"user_id:3671465987;id:gwh-e59905e1-5645-423f-ba70-cac9a88f5157;uniq:fa351b2d-5173-4576-88ab-7f3962e91f0b\",\"pending_object_ids\":[]}" 
    << endl;

    return 0;
}