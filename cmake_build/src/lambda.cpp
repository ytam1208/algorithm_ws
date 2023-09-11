#include <iostream>
#include <thread>
#include <functional>
#include <vector>

class My_class
{
private: std::vector<std::thread> threads;

public:
    void do_something(const std::string& input){std::cout << input.c_str() << std::endl;}
    void do_caller(void){
        std::string input = "hello";

        auto output = [this, input](){return do_something(input);};
        Create_thread<decltype(output)>(output);
    }

    template <typename T1>
    void Create_thread(T1 functor)
    {
        threads.push_back(std::move(std::thread([&](){functor;})));
    }
    My_class()=default;
    ~My_class(){
        std::for_each(threads.begin(), threads.end(), [this](std::thread& thread){thread.join();});
    };
};

int main()
{
    My_class mc;
    mc.do_caller();
    return 0;
}