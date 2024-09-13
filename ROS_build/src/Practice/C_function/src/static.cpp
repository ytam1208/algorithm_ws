#include <iostream>
#include <math.h>

int func(void)
{
    int local_variable = 3;
    local_variable++;
    return local_variable;
}

int static_func(void)
{
    static int local_variable = 3;
    local_variable++;
    return local_variable;
}

int duplicate(int n)
{
    return std::pow(n, 2);
}

int test_case(int n)
{
    // static int a = duplicate(n); //worst case
    static int a; 
    a = duplicate(n);
    return a;
}

int main(int argc, char** argv)
{
    std::cout << "Test Case1 = " << func() << std::endl;
    std::cout << "Test Case2 = " << func() << std::endl;
    std::cout << "Test Case3 = " << func() << std::endl;

    std::cout << "Test Case4 = " << static_func() << std::endl;
    std::cout << "Test Case5 = " << static_func() << std::endl;
    std::cout << "Test Case6 = " << static_func() << std::endl;
    std::cout << "Test Case7 = " << test_case(static_func()) << std::endl;
    std::cout << "Test Case8 = " << test_case(test_case(static_func())) << std::endl;

    return 0;
}