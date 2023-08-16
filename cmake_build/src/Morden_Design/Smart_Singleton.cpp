#include <iostream>
#include <thread>
#include <memory>
#include <string.h>
#include <mutex>

class Singleton
{
protected:
    std::string this_name;
public:
    ~Singleton() = default;
    static std::shared_ptr<Singleton> Get_instance(void)
    {
        std::mutex my_mutex;
        std::lock_guard<std::mutex> lock(my_mutex);
        static std::shared_ptr<Singleton> instance = nullptr;
        if(instance == nullptr){
            instance = std::shared_ptr<Singleton>(new Singleton());
        }
        return instance;
    }
private:
    Singleton() = default;
};

int main(int argc, char** argv)
{
    std::cout << "Creating..instance" << std::endl;
    std::shared_ptr<Singleton> T = Singleton::Get_instance();
    std::cout << "Instance address = " << T << std::endl; 
    T.reset();
    std::cout << "Instance address = " << T << std::endl; 
   
    return 0;
}