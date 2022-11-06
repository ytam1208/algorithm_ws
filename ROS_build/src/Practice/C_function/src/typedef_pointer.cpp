#include <iostream>

class Data
{
    private:
        int Mode;
        typedef void(Data::*FunctionPointer)(int, double);
        FunctionPointer My_mode[3];

    public:
        void func1(int a, double b);
        void func2(int a, double b);
        void runloop();
        void init_mode();
        
        Data():Mode(0)
        {
            init_mode();
            runloop();
        };
        ~Data(){};
};

void Data::init_mode()
{
    My_mode[0] = {&Data::func1};
    My_mode[1] = {&Data::func2};
}

void Data::func1(int a, double b)
{
    std::cout << "(+)Print = " << a + b << std::endl;
}

void Data::func2(int a, double b)
{
    std::cout << "(-)Print = " << a - b << std::endl;
}

void Data::runloop()
{
    int a;
    double b;
    while(1)
    {
        std::cout << "a =";
        std::cin >> a;
        std::cout << "b =";
        std::cin >> b;

        this->Mode = a;
        (this->*My_mode[Mode])(a, b);
    }
}

int main()
{
    Data data;
    return 0;
}