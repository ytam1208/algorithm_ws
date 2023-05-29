#include <iostream>
#include <unordered_map>

int main(int argc, char** argv)
{
    std::unordered_map<std::string, int> hs;
    hs.insert(std::make_pair<std::string, int>("1", 1));
    hs.insert(std::make_pair<std::string, int>("2", 2));
    hs.insert(std::make_pair<std::string, int>("3", 3));
    hs.insert(std::make_pair<std::string, int>("4", 4));
    hs.insert(std::make_pair<std::string, int>("5", 5));
    hs.insert(std::make_pair<std::string, int>("6", 11));
    hs.insert(std::make_pair<std::string, int>("7", 7));
    hs.insert(std::make_pair<std::string, int>("8", 8));
    hs.insert(std::make_pair<std::string, int>("9", 9));
    hs.insert(std::make_pair<std::string, int>("10", 10));
    
    for(auto iter : hs)
        std::cout << iter.first << ", " << iter.second << std::endl;

    //element access with key
    std::cout << "element access" << std::endl;
    std::cout << hs["1"] << std::endl;
    std::cout << hs["10"] << std::endl;

    //add value with the same key
    std::cout << "insert with the same key" << std::endl;
    hs.insert(std::make_pair<std::string, int>("1", 100)); //dosen`t
    std::cout << hs["1"] << std::endl;

    //update key with new value
    std::cout << "update key with new value" << std::endl;
    // auto value = hs.find("1");
    std::unordered_map<std::string, int>::iterator value = hs.find("1");
    if(value != hs.end())
        hs.erase("1");
    std::cout << hs["1"] << std::endl;

    // for(auto iter : hs)
    //     std::cout << iter.first << ", " << iter.second << std::endl;

    return 0;
}