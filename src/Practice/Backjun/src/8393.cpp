#include<iostream>

int main(int argv, char **argc)
{
    int num;
    int a = 0;
    std::cin >> num;
    for(int i = 0; i < num; i++)
    {
      a = i + 1 + a; 
    }
    std::cout << a;
    return 0;
}