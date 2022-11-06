#include <iostream>

class Data
{
    public:
        int Mode;
        // typedef void(Data::*FunctionPointer)(int, double);
        // FunctionPointer My_mode[3];

        // using FunctionPointer = void(Data::*)(int, double);
        // FunctionPointer My_mode[3];

        template <typename T, typename T2> using FunctionPointer = void(Data::*)(T, T2);
        FunctionPointer<int, double> My_mode2[3];


        template <typename T, typename T2>void func1(T a, T2 b);
        template <typename T, typename T2>void func2(T a, T2 b);
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
    // My_mode[0] = {&Data::func1};
    // My_mode[1] = {&Data::func2};

    My_mode2[0] = {&Data::func2};
    My_mode2[1] = {&Data::func2};
}

template <typename T, typename T2>
void Data::func1(T a, T2 b)
{
    std::cout << "(+)Print = " << a + b << std::endl;
}

template <typename T, typename T2>
void Data::func2(T a, T2 b)
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
        // (this->*My_mode[Mode])(a, b);
        (this->*My_mode2[Mode])(a, b);        
    }
}

int main()
{
    Data data;
    Data data2;
    return 0;
}