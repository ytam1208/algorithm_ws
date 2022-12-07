#include "system.hpp"

class node
{
    public:
        int idx_i;
        double idx_d;
        float idx_f;
    node(){}
    ~node(){}
};

void Pointer(std::vector<node>* input1, std::vector<node *> input2)
{
    std::cout << "vector pointer" << std::endl;
    for(auto iter : *input1){
        iter.idx_i = 1;
        std::cout <<  iter.idx_i << " ";
    }
    std::cout << std::endl;

    std::cout << "pointer node" << std::endl;
    for(auto* iter : input2){
        iter->idx_i = 1;
        std::cout << iter->idx_i << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

}

void Print(std::vector<node>* input1, std::vector<node *> input2)
{
    std::cout << "vector pointer" << std::endl;
    for(auto iter : *input1)
        std::cout << iter.idx_i << " ";
    std::cout << std::endl;

    std::cout << "pointer node" << std::endl;
    for(auto* iter : input2)
        std::cout << iter->idx_i << " ";
    std::cout << std::endl;
}

int main()
{
    node my_node;
    node * point_node = &my_node;
    my_node.idx_i = 0;
    my_node.idx_d = 0.0;
    my_node.idx_f = 0.0f;

    std::vector<node> my_vector = {my_node, my_node, my_node, my_node, my_node};
    std::vector<node *> my_pointer;
    for(int i = 0; i < my_vector.size(); i++){
        my_pointer.push_back(&my_vector[i]);
    }

    Pointer(&my_vector, my_pointer);
    Print(&my_vector, my_pointer);

    return 0;
}