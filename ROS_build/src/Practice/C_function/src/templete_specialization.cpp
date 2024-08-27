#include <iostream>
#include <vector>
#include <algorithm>

template <typename T1, typename T2>
class My_Adapter
{ 
    using is_specializaed = std::false_type;
    std::vector<T1> data;

    void Print(void)
    {
        std::for_each(data.begin(), data.end(), [this](const T1& instance){
            std::cout << instance << std::endl;
        });
    }
};

template <>
class My_Adapter<int, double>
{
    using is_specializaed = std::true_type;
    using custom_type1 = int;
    using custom_type2 = double;

    std::vector<custom_type1> data;
    void Print(void)
    {
        std::for_each(data.begin(), data.end(), [this](const custom_type1& instance){
            std::cout << instance << std::endl;
        });
    }
public:
    void check_Specializaed()
    {
        if(is_specializaed::value)
        {
            std::cout << "Specializaed" << std::endl;
        }
        else
        {
            std::cout << "Not Specializaed" << std::endl;
        }
    }
    void check_type()
    {
        std::cout << "custom_type1 = " << typeid(custom_type1).name() << std::endl;
        std::cout << "custom_type2 = " << typeid(custom_type2).name() << std::endl;
    }
};

template <>
class My_Adapter<int*, double*>
{
    using is_specializaed = std::true_type;
    using custom_type1 = int*;
    using custom_type2 = double*;

    std::vector<custom_type1> data;
    void Print(void)
    {
        std::for_each(data.begin(), data.end(), [this](const custom_type1& instance){
            std::cout << *instance << std::endl;
        });        
    }
    void check_type()
    {
        std::cout << "custom_type1 = " << typeid(custom_type1).name() << std::endl;
        std::cout << "custom_type2 = " << typeid(custom_type2).name() << std::endl;
    }
};

int main(int argc, char** argv)
{
    My_Adapter<int, double> test;
    test.check_Specializaed();
    test.check_type();
    return 0;
}