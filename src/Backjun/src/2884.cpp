#include <iostream>
int main(int argv, char **argc)
{
    int H, M;
    int tmp_H = 23;
    int tmp_M;
    std::cin >> H >> M;

    if (M < 45)
    {
        tmp_M = 60 + (M - 45);
        if (H == 0)
        {
            printf("%d %d", tmp_H, tmp_M);
        }
        else
        {
            printf("%d %d", H-1, tmp_M);
        }
    }
    else if (M >= 45)
    {
        tmp_M = M - 45;
        printf("%d %d", H, tmp_M);
    }
    return 0;
}