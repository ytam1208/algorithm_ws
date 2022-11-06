#include <iostream>
#include <vector>
#include <chrono>

int main()
{
    std::vector<int> test_array(10000000000);

    int count = 0;
    clock_t st1 = clock();
    for(int i = 0; i < test_array.size(); i++){
        count++;
    }
    clock_t end1 = clock();
    std::cout << end1 - st1 << std::endl;

    clock_t st2 = clock();
    int len = test_array.size();
    for(int i = 0; i < len; i++){
        count++;
    }
    clock_t end2 = clock();
    std::cout << end2 - st2 << std::endl;


    clock_t st3 = clock();
    for(int i = test_array.size(); i >= 0; i--){
        count++;
    }
    clock_t end3 = clock();
    std::cout << end3 - st3 << std::endl;

    return 0;
}
