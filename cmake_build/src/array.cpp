#include <iostream>
#include <array>
#include <functional>

template <typename T1>
class My_class
{
public: std::array<T1, 3> my_array;
public: std::array<T1, 3> Summation(T1&& input){
    T1 query = std::forward<T1>(input);

    my_array = std::array<T1, 3>{0, };
    std::for_each(my_array.begin(), my_array.end(), [&](T1 element){std::cout << "init element = " << element << std::endl;});
    
    std::for_each(my_array.begin(), my_array.end(), [&, query](T1& element){
        element = query; 
        std::cout << "insert element = " << element << std::endl;
    });
    return this->my_array;
};
public: void Pointer_function(T1* data, std::size_t size){
    for(int iter = 0; iter < size; iter++){
        std::cout << "Pointer vector = " << data[iter] << std::endl;
    }
}

public: My_class()=default;
public: ~My_class()=default;
};

int main(int argc, char** argv)
{
    My_class<double> mc;
    decltype(mc.my_array) ouput = mc.Summation(10);
    mc.Pointer_function(ouput.data(), ouput.size());
    return 0;
}