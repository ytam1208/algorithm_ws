#include<iostream>

int IncreaseNumber(int n)
{
    if(n < 0)
        throw n;
    else if(n == 0)
        throw "0은 입력할 수 없습니다.";
    return ++n;
}

int main()
{
    int num;

    std::cout << "양의 정수를 하나 입력해주세요!" << std::endl;
    while (std::cin >> num)
    {
       try
       {
           std::cout << "입력한 정수에서 1을 증가시킨 Value: " << IncreaseNumber(num) << std::endl;
       }
       catch(int num)
       {
           std::cout << num << "은 양수가 아닙니다." << std::endl;
           std::cout << "양의 정수를 다시 입력해주세요.";
           continue;
       }
       catch(const char* st)
       {
           std::cout << st << std::endl;
           std::cout << "양의 정수를 다시 입력해주세요." << std::endl;
           continue;
       }
       break;
    }
    return 0;
}