#include <iostream>

class MyClass
{
    private:
        int num1;
        int num2;

    public:
        MyClass(int num1, int num2)
        {
            printf("Not this pointer reference!!!\n");
            GetInfo();
            this->num1 = num1;
            this->num2 = num2;
            printf("this pointer reference!!!\n");
            GetInfo();

        }
        void GetInfo()
        {
            std::cout << "num1: " << num1 << std::endl;
            std::cout << "num2: " << num2 << std::endl;
        }
};

int main()
{
    MyClass my(10, 20);
    return 0;
}