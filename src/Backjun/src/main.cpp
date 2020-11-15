// /*
// 정수 X에 사용할 수 있는 연산은 다음과 같이 세 가지 이다.
// 1. X가 3으로 나누어 떨어지면, 3으로 나눈다.
// 2. X가 2로 나누어 떨어지면, 2로 나눈다.
// 3. 1을 뺀다.
// 정수 N이 주어졌을 때, 위와 같은 연산 세 개를 적절히 사용하여 1을 만들려고 한다.
// 연산을 사용하는 횟수의 최솟값을 출력하시오.

// 입력:
// 첫째 줄에 1보다 크거나 같고, 10^6 보다 작거나 같은 정수 N이 주어진다.

// 출력:
// 첫째 줄에 연산을 하는 횟수의 최솟값을 출력한다.(몇번 돌렷는지...)
// */

#include <iostream>
using namespace std;

int arr[10000001];

int check_min(int a, int b)
{
	return (a >= b) ? b : a;
}

int	check(int i, int n)
{
	if (i <= n)
	{
		if (i % 6 == 0)
			arr[i] = check_min(check_min(arr[i / 3], arr[i / 2]), arr[i - 1]) + 1;
		else if (i % 2 == 0)
			arr[i] = check_min(arr[i / 2], arr[i - 1]) + 1;
		else if (i % 3 == 0)
			arr[i] = check_min(arr[i / 3], arr[i - 1]) + 1;
		else
			arr[i] = arr[i - 1] + 1;
		check(i + 1, n);
	}
	if (i == 2)
		return arr[n];
}

void ft_print(int n)
{
	cout << n << endl;
}

int main()
{
	int n;

	cin >> n;
	if (n == 1)
		arr[1] = 0;
	else
		check(2, n);
		
	ft_print(arr[n]);
	return (0);
}