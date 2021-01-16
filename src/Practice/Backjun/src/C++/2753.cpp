#include <iostream>

int main(int argv, char **argc)
{
    int num = 0;
    std::cin >> num;

    if ((num % 4) == 0 && (num % 100) != 0)
    {
        printf("%d", 1);
    }
    else if((num % 400) == 0)
    {
        printf("%d", 1);
    }
    else
    {
        printf("%d", 0);
    }

    return 0;
}