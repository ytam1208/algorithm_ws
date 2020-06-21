#include<iostream>

int main(int argv, char ** argc)
{
    int a[5];
    int total = 0;
    for(int i = 0; i < sizeof(a) / sizeof(int); i++)
    {
        std::cin >> a[i];
        if(a[i] < 40)
        {
            a[i] = 40;
        }
    }
    for(int j = 0; j < sizeof(a) / sizeof(int); j++)
    {
        total = total + a[j];
    }
    std::cout << total/5;
    return 0;
}