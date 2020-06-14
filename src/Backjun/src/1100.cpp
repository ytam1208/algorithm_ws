#include <iostream>
#include <string>

int main()
{
    int count = 0;

    char input;
    char chess_origin[8][8];

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            input = std::cin.get();
            if (input != '\n')
            {
                chess_origin[i][j] = input;
            }
            else
            {
                --j;
            }
        }
    }

    for (int g = 0; g < 8; g++)
    {
        for (int k = 0; k < 8; k++)
        {
            if ((g % 2) == 0 || g == 0)
            {
                if((k % 2) == 0 || k == 0)
                if ((chess_origin[g][k] == 'F'))
                {
                    count++;
                }
            }
            else
            {
                if((k % 2) != 0)
                if ((chess_origin[g][k] == 'F'))
                {
                    count++;
                }
            }

        }
    }

    printf("%d\n", count);

    return 0;
}

// #include<iostream>
// using namespace std;

// int main()
// {
//     int horse = 0;
//     int white = 1;

//     for(int i = 0; i < 8; i++)
//     {
//         for(int j = 0; j < 8; j++)
//         {
//             char c;
//             cin >> c;

//             if(white && c == 'F')
//             horse++;

//             white = !white;

//         }
//         white = !white;

//     }
// }