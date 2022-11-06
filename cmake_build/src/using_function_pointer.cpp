#include <iostream>

template <typename T1, typename T2, typename T3> 
class Data
{
    public:
        using FunctionPointer = T3(Data::*)(T1, T2);
        
        FunctionPointer My_mode[4];

        inline T3 func1(T1 a, T2 b);
        inline T3 func2(T1 a, T2 b);
        inline T3 func3(T1 a, T2 b);
        inline T3 func4(T1 a, T2 b);

        inline void outStream(T1 a, T2 b, T3 mode);

        void init_mode();
        void runloop(T1 a, T2 b);

        Data(){}
        Data(T1 a, T2 b, T3 flag){
            if(flag){
                init_mode();
                runloop(a, b);
            }
        }
        ~Data(){}
};

template <typename T1, typename T2, typename T3> 
inline T3 Data<T1, T2, T3>::func1(T1 a, T2 b){
    return a + b;
}

template <typename T1, typename T2, typename T3>
inline T3 Data<T1, T2, T3>::func2(T1 a, T2 b){
    return a - b;
}

template <typename T1, typename T2, typename T3>
inline T3 Data<T1, T2, T3>::func3(T1 a, T2 b){
    return a / b;
}

template <typename T1, typename T2, typename T3>
inline T3 Data<T1, T2, T3>::func4(T1 a, T2 b){
    return a * b;
}

template <typename T1, typename T2, typename T3>
inline void Data<T1, T2, T3>::outStream(T1 a, T2 b, T3 mode){
    std::cout << (this->*My_mode[mode])(a, b) << std::endl;
}

template <typename T1, typename T2, typename T3> 
void Data<T1, T2, T3>::init_mode()
{
    My_mode[0] = {&Data::func1};
    My_mode[1] = {&Data::func2};
    My_mode[2] = {&Data::func3};
    My_mode[3] = {&Data::func4};    
}

template <typename T1, typename T2, typename T3>
void Data<T1, T2, T3>::runloop(T1 a, T2 b){
    int count = 0;
    for(const auto& node : My_mode){
        outStream(a, b, count);
        count++;
    }
}

template <typename T1, typename T2>
class input_data
{
    public:
        T1 a;
        T2 b;
        
        bool operator()(const T1 input_a, const T2 input_b){
            if(input_a == NULL || input_b == NULL)
                return false;
            return true;
        }
};

int main()
{
    int a, b;

    std::cout << "init a, b = ";
    std::cin >> a >> b;
    std::cout << std::endl;

    input_data<int, int> i_d;
    Data<int, int, int> data(a, b, i_d(a, b));

    return 0;
}

// int main()
// {
//     int a, b, mode;

//     std::cout << "init a, b, mode = ";
//     std::cin >> a >> b >> mode;
//     std::cout << std::endl;
    
//     Data<int, int, int> data;
//     switch (mode)
//     {
//     case 0:
//         data.func1(a, b);   //a + b
//         break;
//     case 1:
//         data.func2(a, b);   //a - b
//         break;    
//     case 2:
//         data.func3(a, b);   //a * b
//         break;    
//     case 3:
//         data.func4(a, b);   //a / b
//         break;    
//     // case 4:
//     //     data.func5(a, b);   //a * a + b * b
//     //     break;    
//     // case 5:
//     //     data.func6(a, b);   //sqrt(pow(a,2)+pow(b,2))
//     //     break;    
//     // case 6:
//     //     data.func7(a, b);   // ...
//     //     break;    
//     // ...
//     default:
//         break;
//     }

//     return 0;
// }