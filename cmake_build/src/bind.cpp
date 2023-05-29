#include <iostream>
#include <functional>

class A
{
public:
    void addAndPrint(int a, int b)
    {
        int sum = a + b;
        std::cout << "sum = " << sum << std::endl;
    }
    void subAndPrint(int a, int b)
    {
        int sub = a - b;
        std::cout << "sub = " << sub << std::endl;
    }
    int mulAndPrint(int a, int b)
    {
        return a * b;
    }
    void bindFunction1(std::function<void(int, int)>& dst)
    {
        dst = std::bind(&A::addAndPrint, this, std::placeholders::_1, std::placeholders::_2);
        dst(10, 10);
        dst = std::bind(&A::subAndPrint, this, std::placeholders::_1, std::placeholders::_2);
        dst(10, 20);
        dst = std::bind(&A::subAndPrint, this, std::placeholders::_2, std::placeholders::_1);
        dst(10, 20);
    }
    void bindFunction2(std::function<int(int, int)>& dst)
    {
        dst = std::bind(&A::mulAndPrint, this, std::placeholders::_1, std::placeholders::_2);
    }
    void bindFunction3(std::function<int(int, int)>& dst)
    {
        // dst = std::bind(&A::mulAndPrint, this, std::placeholders::_1, std::placeholders::_2);
        // dst = std::bind(&A::mulAndPrint, this, std::placeholders::_2, std::placeholders::_1);
        dst = std::bind(&A::mulAndPrint, this, std::placeholders::_2, 100);
        // dst = std::bind(&A::mulAndPrint, this, std::placeholders::_3, 100); compile error
    }
};

int main(int argc, char** argv)
{
    A test;
    std::function<void(int, int)> functor;
    test.bindFunction1(functor);
    functor(10, 20);

    std::function<int(int, int)> mul_functor;
    test.bindFunction2(mul_functor);
    std::cout << "mul = " << mul_functor(10, 10) << std::endl;

    test.bindFunction3(mul_functor);
    std::cout << "mul = " << mul_functor(10, 10) << std::endl;

    std::function<void(int,int)> adder = std::bind(&A::addAndPrint, &test, std::placeholders::_1, std::placeholders::_2);
    adder(10,10);

    return 0;
}