#include <iostream>

int main(int argc, char **argv)
{
    int a, b, c;
    int tmp = 0;
    int count = 0;

    std::cin >> a >> b >> c;
    tmp = a * b * c;
    
    while (tmp != 0)
    {
        count = count + 1;
        tmp = tmp / 10;
    }
    tmp = a * b * c;
    int arr[count] = {0, };
    int num[10] = {0, };

    for (int i = 0; i < count; i++)
    {
        arr[i] = tmp % 10;
        switch (arr[i])
        {
        case 0:
            num[0] += 1;
            break;
        case 1:
            num[1] += 1;
            break;
        case 2:
            num[2] += 1;
            break;
        case 3:
            num[3] += 1;
            break;
        case 4:
            num[4] += 1;
            break;
        case 5:
            num[5] += 1;
            break;
        case 6:
            num[6] += 1;
            break;
        case 7:
            num[7] += 1;
            break;
        case 8:
            num[8] += 1;
            break;
        case 9:
            num[9] += 1;
            break;
        }
        tmp = tmp/10;
    }
    for(int i = 0; i < 10; i++)
        std::cout << num[i] << std::endl;

    return 0;
}
