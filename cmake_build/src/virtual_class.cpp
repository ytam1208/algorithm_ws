#include "system.hpp"

template <typename T1, typename T2>
class Parent
{
    public:
    T1 name;
    T2 age;

        inline T1 get_name(){
            return this->name;
        }
        virtual inline T2 get_age(){
            return this->age;
        }
        
        Parent(){}
        Parent(T1 input_name, T2 input_age):name(input_name),age(input_age){}
        ~Parent(){
            std::cout << "Parent: destroy " << this->name << " Class" << std::endl;
        }
};

template <typename T1, typename T2>
class Child : public Parent<T1, T2>
{
    public:
    T1 child_name;
    T2 child_age;

        virtual inline int get_age(){
            return this->child_age * this->child_age;
        }

        Child(){}
        Child(T1 input_n, T2 input_a):child_name(input_n),child_age(input_a){}
        // Child(T1 input_n, T2 input_a):Parent<T1, T2>(input_n, input_a),child_name(input_n),child_age(input_a){}
        ~Child(){
            std::cout << "Child: destroy " << this->child_name << " Class" << std::endl;
        }
};

int main()
{
    Parent<std::string, int> *tamine = new Parent<std::string, int>("tam", 20);
    std::cout << tamine->get_name() << std::endl;
    std::cout << tamine->get_age() << std::endl;
    
    Child<std::string, int> *son = new Child<std::string, int>("son", 10);
    std::cout << son->get_name() << std::endl;
    std::cout << son->get_age() << std::endl;

    std::cout << "Virutal class function" << " [Parent] class = " <<  tamine->get_age() << " ---> [Child] class = " <<  son->get_age() << std::endl;

    delete tamine;
    delete son;

    return 0;
}