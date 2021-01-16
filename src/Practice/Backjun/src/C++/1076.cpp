#include <string>
#include <iostream>
#include <math.h>

int main()
{
	std::string color;
	std::string ary[10] = {"black", "brown", "red", "orange", "yellow", "green", "blue", "violet", "grey", "white"};
	long long num[3] = {0, };
	
	for(int i = 0; i < 3; i++)
	{
		std::cin >> color;
		for(int j = 0; j < 10; j++)
		{
			if(ary[j] == color)
			num[i] = j;
		}
	}

	std::cout << (num[0]*10 + num[1]) * (long long)pow(10,num[2]); 


	return 0;
}

