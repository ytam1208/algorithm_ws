#include <iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    int a, b, num;
    std::cin >> num;

    for(int i = 0; i < num; i++)
    {
    std::cin >> a >> b;
    std::cout << "Case" << " #" << i+1 << ": " << a << " + " << b << " = " << a+b << '\n';
    }
    
    return 0;
}