#include<iostream>
int main(int argv, char **argc)
{
    int num, num2;

    std::cin >> num >> num2;

    if(num >= 0 && num2 >= 0)
    {
        printf("1");
    }
    else if(num < 0 && num2 >= 0)
    {
        printf("2");
    }
    else if(num < 0 && num2 < 0)
    {
        printf("3");
    }
    else if(num >= 0 && num2 < 0)
    {
        printf("4");
    }
    return 0;  
}