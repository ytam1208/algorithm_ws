#include<iostream>

int main(int argv, char **argc)
{
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);
    int num, num2;

    std::cin >> num >> num2;
    int num3[num];

    for(int i = 0; i < num; i++)
    {
        std::cin >> num3[i];
    }
    for(int j = 0; j < num; j++)
    {
         if(num3[j] < num2)
         {
            std::cout << num3[j] << " ";
        }
    }
    return 0;
}