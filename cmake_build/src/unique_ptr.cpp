#include "system.hpp"
#include <memory>

template <typename T1, typename T2, typename T3>
class Parent
{
    public:
        T1 name;
        T2 age;
        T3 weight;

    public:
        virtual inline T1 get_name(){
            return this->name;
        }
        virtual inline T2 get_age(){
            return this->age;
        }
        virtual inline T3 get_weight(){
            return this->weight;
        }

        Parent(){}
        Parent(T1 i_n, T2 i_a, T3 i_w):name(i_n),age(i_a),weight{i_w}{}
        ~Parent(){
            std::cout << "Destroy " << this-> name << " Class" << std::endl;
        }  
};

template <typename T1, typename T2, typename T3>
class Child : public Parent<T1, T2, T3>
{
    T1 name;
    T2 age;
    T3 weight;

    public:
        virtual inline T3 get_weight(){
            return this->weight/(T3)2;
        }        

        Child(T1 i_n, T2 i_a, T3 i_w):name(i_n), age(i_a), weight(i_w){};
        ~Child(){
            std::cout << "Destroy " << this->name << " Class" << std::endl;
        }
};

int main()
{
    Parent<std::string, int, double> *tamin = new Parent<std::string, int, double>("tam", 20, 80.0);
    std::cout << tamin->get_name() << std::endl;
    std::cout << tamin->get_age() << std::endl;
    std::cout << tamin->get_weight() << std::endl;
    delete tamin;

    std::cout << std::endl;

    //C++ 11 new -> std::unique_ptr
    // std::unique_ptr<Parent<std::string, int, double>> share_tm {new Parent<std::string, int, double>("share_tm", 20, 80)};
    
    //C++ 14 make_unique -> std::unique_ptr
    std::unique_ptr<Parent<std::string, int, double>> share_tm = std::make_unique<Parent<std::string, int, double>>("share_tm", 20, 80);
    std::cout << share_tm->get_name() << std::endl;
    std::cout << share_tm->get_age() << std::endl;
    std::cout << share_tm->get_weight() << std::endl;

    std::cout << std::endl;

    //유니크 포인터는 복사, 대입이 불가능함 아래 코드는 컴파일 에러
    //std::unique_ptr<Parent<std::string, int, double>> copy = share_tm;

    //std::move를 사용하여 메모리를 copy로 이동시키고 기존 share_tm은 nullptr를 갖는다.
    std::unique_ptr<Parent<std::string, int, double>> copy = std::move(share_tm);
    std::cout << copy->get_name() << std::endl;
    std::cout << copy->get_age() << std::endl;
    std::cout << copy->get_weight() << std::endl;
    
    //unique_pointer 멤버함수 get으로 원초적인 주소 값에 있는 변수 name 변경 가능
    copy.get()->name = "get_chage_name";
    std::cout << copy->get_name() << std::endl;

    Parent<std::string, int, double> *tmp_t = copy.release(); //releas를 사용하여 원시 포인터의 주소 값을 저장함.
    tmp_t->name = "chage tmp_name";
    std::cout << tmp_t->get_name() << std::endl;
    // std::cout << copy->get_name() << std::endl; // 원시 포인터의 주소 값을 저장하는 std::unique_ptr의 copy는 위 release에 의해 nullptr를 가지게 된다.
    copy.reset();   //원시 포인터를 소멸시킴으로써 unique_ptr copy는 소멸되어 더이상 사용 못함.

    return 0;
}