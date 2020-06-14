#include <iostream>

int main(int argv, char **argc)
{
    int num;
    std::cin >> num;
    if (90 <= num && num <= 100)
    {
        std::cout << "A" << std::endl;
    }
    else if (80 <= num && num < 90)
    {
        std::cout << "B" << std::endl;
    }
    else if (70 <= num && num < 80)
    {
        std::cout << "C" << std::endl;
    }
    else if (60 <= num && num < 70)
    {
        std::cout << "D" << std::endl;
    }
    else
    {
        std::cout << "F" << std::endl;
    }
    return 0;
}