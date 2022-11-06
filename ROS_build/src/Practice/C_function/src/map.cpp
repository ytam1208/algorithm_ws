#include <map>
#include <iostream>

int main()
{
    std::map<int,int> first;
    first.insert(std::make_pair(1,1));
    first.insert(std::make_pair(2,2));
    first.insert(std::make_pair(3,4));
    first.insert(std::make_pair(4,8));
    first.insert(std::make_pair(5,16));
    first[10] = 200;

    std::cout << "first_size = " <<first.size() << std::endl;

    for(auto a : first)
        std::cout << "first = " << a.first << " " << "second = " << a.second << std::endl;
    
    //find() , use key index -> value index
    std::cout << first.find(4)->second << std::endl;
    
    std::cout << first.count(2) << std::endl;

    first.clear();
    std::cout << "first_size = " << first.size() << std::endl;
}