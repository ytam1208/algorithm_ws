#include <iostream>

int main(int argv, char **argc)
{
    int a = 0;
    int b = 0;
    double tmp = 0;
    std::cin >> a >> b;
    tmp = (double)a / (double)b;
    std::cout.precision(10);
    std::cout << tmp;

    return 0;
}