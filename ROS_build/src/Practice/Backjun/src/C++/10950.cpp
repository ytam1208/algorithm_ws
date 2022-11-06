#include <iostream>

int main(int argv, char **argc)
{
    int a,b,num;
    std::cin >> num;
    for(int i = 0; i < num; i++)
    {
        std::cin >> a >> b;
        printf("%d\n", a+b);
    }
    return 0;
}