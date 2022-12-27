#include <iostream>
#include <vector>
#include <chrono>
#include <ros/ros.h>

int main()
{
    std::vector<int> test_array(10);
    clock_t t1(0);
    clock_t t2(0);
    clock_t t3(0);
    int loop_count(0);

    while(1){
        
        int count = 0;
        int count2 = 0;
        int count3 = 0;

        clock_t st1 = clock();
        for(int i = 0; i < test_array.size(); i++)
            count++;
        clock_t end1 = clock();

        clock_t st2 = clock();
        int len = test_array.size();
        for(int i = 0; i < len; i++)
            count2++;
        clock_t end2 = clock();

        clock_t st3 = clock();
        for(int i = test_array.size(); i >= 0; i--)
            count3++;
        clock_t end3 = clock();

        t1 += (end1 - st1);
        t2 += (end2 - st2);
        t3 += (end3 - st3);
        std::cout << "loop 1 = " <<  t1 << "ms" << std::endl;
        std::cout << "loop 2 = " <<  t2 << "ms" << std::endl;
        std::cout << "loop 3 = " <<  t3 << "ms" << std::endl;   
    }

    return 0;
}