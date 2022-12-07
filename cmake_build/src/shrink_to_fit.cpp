#include "system.hpp"

int main()
{
    std::vector<int> node = {1,2,3,4,5};
    std::vector<int> node2 = {1,1,1,1,1,1,1,1};
    std::cout << "node2 = " << node2.capacity() << std::endl;

    // std::vector<int>().swap(node2);
    // // node2.clear();
    // node2.resize(node.size());
    node2.clear();
    node2.shrink_to_fit();
    node2.resize(node.size());

    std::cout << "node2 = " << node2.capacity() << std::endl;
    std::copy(node.begin(), node.end(), node2.begin());
    for(auto iter : node2)
        std::cout << iter << std::endl;
    return 0;  
}