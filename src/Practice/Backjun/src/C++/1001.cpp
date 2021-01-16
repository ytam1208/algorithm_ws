#include <iostream>

int main(int argv, char **argc)
{
    int a, b;
    while (1)
    {
        std::cin >> a >> b;
        if (0 < a && b < 10)
        {
            printf("%d", a - b);
            break;
        }
    }
}