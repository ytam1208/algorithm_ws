#include <iostream>

//////////////////////////////////함수 템플릿
// template<typename T>
// void Swap(T& num1, T&num2)
// {
//     T temp = num1;
//     num1 = num2;
//     num2 = temp;
// }

// int main()
// {
//     int num1 = 10, num2 = 40;
//     std::cout << "before, num1: " << num1 << ", num2: " << num2 << std::endl;
//     Swap(num1, num2);
//     std::cout << "after, num1: " << num1 << ", num2: " << num2 << std::endl;

//     int num3 = 15.6, num4 = 14.44;
//     std::cout << "before, num3: " << num3 << ", num4: " << num4 << std::endl;
//     Swap(num3, num4);
//     std::cout << "after, num3: " << num4 << ", num4: " << num4 << std::endl;

//     return 0;
// }

//////////////////////////////////클래스 템플릿
// template <typename T>
class Data
{
    private:
        // T data;
    public:
        // Data(T data): data(data){}
        // void ShowInfo()
        // {
        //     std::cout << data << std::endl;
        // }
        template <typename TL>
        void Go(TL &input, TL &output)
        {
            TL compare;
            compare.x = input.x - output.y;
            std::cout << "compare x = " << compare.x << std::endl;
        }
        Data(){};
        ~Data(){};
};

class A
{
    public:
        int x = 100;
        int y = 20;
};

int main()
{
    // Data<int> data1(50);
    // data1.ShowInfo();

    // Data<char> data2('A');
    // data2.ShowInfo();

    // Data<double> data3(24.377);
    // data3.ShowInfo();

    Data data4;
    A a;
    data4.Go(a, a);
}