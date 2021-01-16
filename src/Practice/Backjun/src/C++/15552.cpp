#include <iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    int num, a, b;
    std::cin >> num;
    for(int i = 0; i < num; i++)
    {
        std::cin >> a >> b;
        std::cout << a+b << '\n';   
    }

    return 0;
}