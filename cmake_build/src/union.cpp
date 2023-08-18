#include <iostream>
#include <mutex>
#include <memory>

union my_union
{
   int dummy_int;
   float dummy_float;
   double dummy_double;
};

class My_class
{
public:
    // union my_union 
    // {
    //     int dummy_int;
    //     float dummy_float;
    //     double dummy_double;
    // };
    my_union mu;

    int dum;
    static std::shared_ptr<My_class> Get_instance(void){
        std::mutex mt;
        std::lock_guard<std::mutex> lg(mt);
        static std::shared_ptr<My_class> Pointer = std::shared_ptr<My_class>(new My_class());
        return Pointer;
    }
    ~My_class() = default;
private:
    My_class() = default;
};

int main(int argc, char** argv)
{
    my_union mu;
    mu.dummy_int = 1;
    std::cout << "int = " << mu.dummy_int << ", size = " << sizeof(mu.dummy_int) <<std::endl;
    mu.dummy_float = 2.f;
    std::cout << "float = " << mu.dummy_float << ", size = " << sizeof(mu.dummy_float) <<std::endl;
    mu.dummy_double = 4.f;
    std::cout << "double = " << mu.dummy_double << ", size = " << sizeof(mu.dummy_double) <<std::endl;
    std::cout << mu.dummy_int << ", " << sizeof(mu.dummy_int) <<std::endl;
    std::cout << mu.dummy_float << ", " << sizeof(mu.dummy_float) <<std::endl;

    std::shared_ptr<My_class> c_p = My_class::Get_instance();
    c_p->mu.dummy_int = 1;
    
    return 0;
}