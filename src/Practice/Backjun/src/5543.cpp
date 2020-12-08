#include <iostream>

int main(int argv, char **argc)
{
    int a[3] = {
        0,
    };
    int b[2] = {
        0,
    };
    int tmp, tmp2;

    for (int i = 0; i < sizeof(a) / sizeof(int); i++)
        std::cin >> a[i];
    for (int i = 0; i < sizeof(b) / sizeof(int); i++)
        std::cin >> b[i];
    tmp = a[0];
    tmp2 = b[0];
    for (int i = 1; i < sizeof(a) / sizeof(int); i++)
        if (tmp > a[i])
            tmp = a[i];
    for (int i = 1; i < sizeof(b) / sizeof(int); i++)
        if (tmp2 > b[i])
            tmp2 = b[i];
    std::cout << tmp + tmp2 - 50;
    return 0;
}