#include <iostream>

int main(int argv, char **argc)
{
    int num, a, b;
    int tmp = 0;
    int count = 0;
    std::cin >> num;
    a = num / 10;
    b = num % 10;

    while (1)
    {
        count++;
        tmp = a + b;
        if(tmp < 10)
        {
          tmp = (b * 10) + tmp;
          a = tmp / 10;
          b = tmp % 10;
        }
        else if(tmp >= 10)
        {
            tmp = (b * 10) + (tmp % 10);
            a = tmp / 10;
            b = tmp % 10;
        }
        if(tmp == num)
        {
            std::cout << count;
            break;
        }
    }
    return 0;
}