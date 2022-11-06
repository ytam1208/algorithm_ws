#include <iostream>

int main(int argv, char **argc)
{
    int a, b;
    while (1)
    {
        std::cin >> a >> b;
        if (a == 0 && b == 0)
            break;
        else
            std::cout << a + b << '\n';
    }

    return 0;
}