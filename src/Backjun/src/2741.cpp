#include<iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int N;

    std::cin >> N;
    for(int i = 0; i < N; i++)
        std::cout << i+1 << '\n';
    return 0;
}