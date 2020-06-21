#include <iostream>

int main(int argv, char **argc)
{
    int a, b;
    while(scanf("%d%d", &a, &b) != EOF)
    {
        std::cout << a + b << '\n';
    }

    return 0;
}