#include <iostream>
#include <map>

int main()
{
    std::multimap<int, int> multi_map;

    multi_map.insert(std::make_pair(1,100));
    multi_map.insert(std::make_pair(2,2));
    multi_map.insert(std::make_pair(3,4));
    multi_map.insert(std::make_pair(4,8));
    multi_map.insert(std::make_pair(5,16));

    multi_map.insert(std::make_pair(1,32));


    for(auto a : multi_map)
        std::cout << a.first << " " << a.second << std::endl;

    while(1)
    {
        if(multi_map.find(1)->second == 32)
            break;
        else
            std::cout << multi_map.find(1)->second << std::endl;    
    }
}