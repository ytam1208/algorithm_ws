#include<iostream>

enum DayOfWeek{
    Sunday = 0,
    Monday, //1
    Tuesday, //2
    Wednesday, //3
    Thursday, //4
    Friday, //5
    Saturday //6
};

DayOfWeek return_fucntion(DayOfWeek input)
{
    return input;
};

int main()
{
    enum DayOfWeek week; // 열거형 변수 선언

    week = Tuesday;    //열거형 value 할당

    printf("%d \n", week);

    std::cout << return_fucntion(week) << std::endl;

    return 0;
}