#include <iostream>

int main(int argv, char **argc)
{
    int static_cost = 0;
    int material = 0;
    int it_cost = 0;
    std::cin >> static_cost >> material >> it_cost;

    int total_cost = 0;
    int tmp_material = material;
    int tmp_it_cost = it_cost;
    int count = 0;
    int *ptr = &count;
    while (1)
    {
        total_cost = static_cost + material;
        if (it_cost > total_cost)
        {
            break;
        }
        else
        {
            it_cost = it_cost + tmp_it_cost;
            material = material * tmp_material;
            ++*ptr;
            //std::cout << "-1" << std::endl;
        }
    }
    std::cout << count << std::endl;

    return 0;
}