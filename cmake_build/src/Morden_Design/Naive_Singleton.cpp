#include <iostream>
#include <memory>
#include <string.h>
#include <mutex>

class Dummy_class
{
protected:
    std::string this_name;
private:
    Dummy_class() = default;

    static Dummy_class* instance;
public:
    ~Dummy_class() = default;
    Dummy_class(const Dummy_class &) = delete;
    Dummy_class(Dummy_class &&) = delete;
    Dummy_class operator=(const Dummy_class &) = delete;
    Dummy_class operator=(const Dummy_class &&) = delete;

    static Dummy_class* Basic_Get_instance(void)
    {  
        return instance;
    }
    static Dummy_class* Dynamic_Get_instance(void)
    {  
        if(instance == nullptr) instance = new Dummy_class(); 
        return instance;
    }
    static Dummy_class* Local_Get_instance(void)
    {
        static Dummy_class local_instance;
        return &local_instance;
    }
    static void Destroy_instance(void)
    {
        if(instance != nullptr)
            delete instance;
        instance = nullptr;
    }
};

Dummy_class* Dummy_class::instance = nullptr;

int main(int argc, char** argv)
{
    std::cout << "Creating..instance" << std::endl;

    auto const* B_T = Dummy_class::Basic_Get_instance();
    auto const* D_T = Dummy_class::Dynamic_Get_instance();
    auto const* L_T = Dummy_class::Local_Get_instance();

    std::cout << "Instance address = " << B_T << std::endl; 
    std::cout << "Instance address = " << D_T << std::endl; 
    std::cout << "Instance address = " << L_T << std::endl; 
    
    return 0;
}