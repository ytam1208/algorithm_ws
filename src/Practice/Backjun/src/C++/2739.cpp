#include <iostream>

int main(int argv, char ** argc)
{
    int num = 0;
    std::cin >> num;
    for(int i = 1; i < 10; i++)
    {
        printf("%d * %d = %d\n", num, i, num*i);
    }
    return 0;
}