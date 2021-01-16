#include<iostream>

int main(int argv, char ** argc)
{
    int max, min, num;

    std::cin >> num;
    int a[num] = {0, };
    
    for(int i = 0; i < num; i++)
    {
        std::cin >> a[i];
    }
    max = a[0];
    min = a[0];
    for(int i = 0; i < num; i++)
    {
        if(max < a[i])
            max = a[i];
        else if(min > a[i])
            min = a[i];
    }
    std::cout << min << ' ' << max;
    return 0;
}