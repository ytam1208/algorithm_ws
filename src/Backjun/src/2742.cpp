#include<iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int n;
    std::cin >> n;
    for(int i = n; i > 0; i--)
        std::cout << i << '\n';
    return 0;

}