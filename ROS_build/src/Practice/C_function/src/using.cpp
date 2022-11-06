#include <iostream>

class A
{
    public:
        double x = 0.0;
        double y = 0.0;
    A()
    {
        std::cout << "x = ";
        std::cin >> x;
        std::cout << "y = ";
        std::cin >> y;
    }
    ~A(){}
};

class Data
{
    public:
        int Mode;
        // typedef void(Data::*FunctionPointer)(int, double);
        // FunctionPointer My_mode[3];

        // using FunctionPointer = void(Data::*)(int, double);
        // FunctionPointer My_mode[3];

        template <typename T, typename T2> using FunctionPointer = void(Data::*)(T, T2);
        FunctionPointer<int, int> My_mode1[3];
        FunctionPointer<int, double> My_mode2[3];
        FunctionPointer<double, int> My_mode3[3];
        FunctionPointer<double, double> My_mode4[3];

        template <typename T, typename T2> void func1(T a, T2 b);
        template <typename T, typename T2> void func2(T a, T2 b);
        template <typename T, typename T2> void runloop(T a, T2 b);
        void init_mode();
        Data():Mode(0)
        {
            // init_mode();
            // runloop();
        };
        ~Data(){};
};

void Data::init_mode()
{
    My_mode1[0] = {&Data::func1};
    My_mode1[1] = {&Data::func2};

    My_mode2[0] = {&Data::func1};
    My_mode2[1] = {&Data::func2};

    My_mode3[0] = {&Data::func1};
    My_mode3[1] = {&Data::func2};

    My_mode4[0] = {&Data::func1};
    My_mode4[1] = {&Data::func2};

    A a;
    runloop<int, int>(a.x, a.y);
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

template <typename T, typename T2>
void Data::runloop(T a, T2 b)
{
    // Mode = 0;
    // (this->*My_mode1[Mode])(10.5, 10.5);       
    // (this->*My_mode2[Mode])(10.5, 10.5);       
    // (this->*My_mode3[Mode])(10.5, 10.5);       
    // (this->*My_mode4[Mode])(10.5, 10.5);       

    // Mode = 1;
    // (this->*My_mode1[Mode])(11.5, 10.5);       
    // (this->*My_mode2[Mode])(11.5, 10.5);       
    // (this->*My_mode3[Mode])(11.5, 10.5);       
    // (this->*My_mode4[Mode])(11.5, 10.5);

    Mode = 0;
    (this->*My_mode4[Mode])(a, b);
   
}

class B
{
    public:
        Data data;

    void runloop()
    {
        data.init_mode();
    }
};

int main()
{
    // Data data;
    B b;
    b.runloop();
    return 0;
}