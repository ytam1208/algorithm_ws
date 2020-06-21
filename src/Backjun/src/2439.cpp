#include <iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int num;
    std::cin >> num;
    for(int i = 1; i <= num; i++)
    {
        for(int k = num - 1; k >= i; k--)
        {
            std::cout << " ";
        }
        for(int j = 1; j <= i ; j++)
        {
            std::cout << "*";
        }
        std::cout << '\n';
    }
    return 0;
}