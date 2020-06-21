#include <iostream>

int main(int argv, char **argc)
{
    int a[10] = {
        0,
    };
    int count = 0;

    for (int i = 1; i < 10; i++)
    {
        std::cin >> a[i];
    }
    int max = a[1];
    for (int j = 1; j < 10; j++)
    {
        if (max <= a[j])
        {
            max = a[j];
            count = j;
        }
    }
    std::cout << max << std::endl << count;
    return 0;
}