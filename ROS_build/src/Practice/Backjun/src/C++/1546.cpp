#include<iostream>

int main()
{
    int num = 0;
    std::cin >> num;
    double a[num] = {0, };
    double b[num] = {0.0, };
    double max = a[0];
    double avr = 0;

    for(int i = 0; i < num; i++)
        std::cin >> a[i];
    for(int j = 0; j < num; j++)
    {
        if(max < a[j])
            max = a[j];
    }
    for(int j = 0; j < num; j++)
    {
        b[j] = (a[j] / max) * 100;
        avr += b[j];
    }
    avr = (double)avr / (double)num;
    std::cout << std::fixed;
    std::cout.precision(7);
    std::cout << avr;
    
    return 0;
}