#include <iostream>
#include <vector>
#include <algorithm>

template <typename... Args>
struct Tuple
{
    void print(Args... args){}
};

template <>
struct Tuple<int, double, bool>{};

template <typename Arg, typename... Args>
struct Tuple_2{};

template <typename... Args, typename Arg>
void valid(Arg, Args...){}
// void valid(Args..., Arg){} //Error

template <typename Arg, typename... Args>
class Template_Pack
{
public:

    void Print(Arg arg)
    {
        std::cout << "In2 = " << arg << std::endl;
    }

    template <typename U, typename... Us>
    void Print(U arg, Us... args)
    {
        std::cout << "In1 = " << arg << ", ";
        Print(std::forward<Us>(args)...);
    }


    void Print_C17(const Arg& arg, const Args&... args)
    {
        std::cout << arg;
        ((std::cout << ", " << args), ...);
        std::cout << std::endl;
    }
};

int main(int argc, char **argv)
{
    Tuple<> t0;
    t0.print();
    Tuple<int> t1;
    t1.print(1);
    Tuple<int, float> t2;
    t2.print(2, 1.0);    
    valid(1.0, 1, 2, 3);

    // Template_Pack<int, double, std::string> tp;
    // tp.Print(1, 2.0, "done");
    Template_Pack<int,int,int,int,int,int,int> tpp;
    tpp.Print(1,2,3,4,5,6,7);
    tpp.Print_C17(8,9,10,11,12,13,14);
    
    return 0;
}