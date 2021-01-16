#include<iostream>

int main(int argv, char **argc)
{
    int a, b;
    while(1)
    {
        std::cin >> a >> b;
        if(1 <= a && b <= 10000)
        {
            printf("%d\n",  a+b);
            printf("%d\n",  a-b);
            printf("%d\n",  a*b);
            printf("%d\n",  a/b);
            printf("%d\n",  a%b);
            break;
        }
    }
    return 0;
}