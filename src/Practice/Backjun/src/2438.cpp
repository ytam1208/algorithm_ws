#include <iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int num;
    std::cin >> num;
    for(int i = 0; i < num; i++)
    {
        for(int j = 0; j <= i; j++)
        {
            std::cout << "*";
        }
        std::cout << '\n';
    }
    return 0;
}