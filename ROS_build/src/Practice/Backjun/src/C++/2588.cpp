#include <iostream>

int main(int argv, char **argc)
{
    int num = 0;
    int num2 = 0;
    int arr[3] = {
        0,
    };
    int arr2[3] = {
        0,
    };

    int count = 1;
    std::cin >> num;
    std::cin >> num2;

    arr[2] = num2 / 100;
    arr[1] = (num2 - (arr[2] * 100)) / 10;
    arr[0] = num2 - ((arr[2] * 100) + (arr[1] * 10));
    int size = sizeof(arr) / sizeof(int);

    for (int i = 0; i < size; i++)
    {
        arr2[i] = arr[i] * num;
        std::cout << arr2[i] << std::endl;
        arr2[i] = arr2[i] * count;
        count = count * 10;
    }
    int total = 0;
    for(int j = 0; j < size; j++)
    {
        total += arr2[j];
    }
    printf("%d", total);

    return 0;
}