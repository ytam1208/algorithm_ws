#include <iostream>
#include <functional>

class Myclass
{
public:
    int value;
    std::string this_name;
    Myclass():this_name("Myclass"){}
    Myclass(int value):this_name("Myclass"){
        this->value = value;
    }
    int foo(int value){
        return value;
    }
    int fifth(void){
        return this->value/5;
    }
    void chageValue(int new_value){
        this->value = new_value;
    }
    void operator()(void){
        std::cout << "Hello my name is " << this_name.c_str() << std::endl;
    }
};

class Manager_class
{
public:
    void Handle(std::function<void(int)>& cf, std::function<int(void)>& f1){
        cf(10);
        std::cout << f1() << std::endl;
        std::cout << "이런식으로 다양하게 함수들을 객체로 바꿔 전달 할 수 있다." << std::endl;
    }
    Manager_class(){}
    ~Manager_class(){}
};

template <typename T1>
class test_class
{
public:
    T1 value;
    T1 print_value(){
        return this->value;
    }

    template <typename T2>
    T2 print_value1(T2 value){
        return value;
    }
    test_class(){}
    test_class(T1 _value):value(_value){}
    ~test_class(){}
};



int main(int argc, char** argv)
{
    Myclass mc(5);
    std::function<int(void)> fn1 = std::bind(&Myclass::fifth, &mc);                         //function
    std::function<int(int)> fn_ = std::bind(&Myclass::foo, &mc, std::placeholders::_1);   
    std::function<void(void)> fn2 = Myclass();                                              //function object
    std::function<int(int)> fn3 = [](int x){return x / 5;};                                 //lambda expression
    
    std::cout << fn1() << std::endl;
    fn2();
    std::cout << fn3(5) << std::endl;
    
    std::function<int(Myclass&)> value = &Myclass::value;                                   //pointer to data member
    std::function<int(Myclass&)> fifth = &Myclass::fifth;                                   //pointer to member function
    std::function<int(Myclass&, int)> foo = &Myclass::foo;
    
    Myclass sixty(60);
    std::cout << value(sixty) << std::endl;
    std::cout << fifth(sixty) << std::endl;
    std::cout << foo(sixty, 100) << std::endl;
    
    Manager_class man;
    std::function<void(int)> cf = std::bind(&Myclass::chageValue, &mc, std::placeholders::_1);
    man.Handle(cf, fn1);

    test_class<int> tc(10);
    test_class<double> tc_d;
    std::function<int(void)> fn4 = std::bind(&test_class<int>::print_value, &tc);
    std::function<int(int)> fn5 = std::bind(&test_class<double>::print_value1<int>, &tc_d, std::placeholders::_1);

    std::cout << fn4() << std::endl;
    std::cout << fn5(100) << std::endl;

    return 0;
}