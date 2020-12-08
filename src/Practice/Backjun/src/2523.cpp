#include<iostream>

int main(int argv, char **argc)
{
    int num;
    std::cin >> num;

    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            std::cout << "*";
        }
        std::cout << '\n';
    }
    int total = 2 * num - 1;
    int count = num - 2;
    for(int k = 0; k < total-num; k++)
    {
        for(int i = 0; i <= count; i++)
        {
            std::cout << "*";
        }
        std::cout << '\n';
        count--;
    }    
    return 0;
}