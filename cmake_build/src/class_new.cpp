#include "system.hpp"

/*
클래스 이름 *포인터 변수 = new 클래스이름;
클래스 이름 *포인터 변수 = new 클래스이름(생성자 매개변수);
*/


class human
{
    std::string name;
    int age;
    double weight;
    
    public:
        inline std::string get_name(){
            return name;
        }
        inline int get_age(){
            return age;
        }
        inline double get_weight(){
            return weight;
        }

        human(std::string i_n, int i_a, double i_w):name(i_n), age(i_a), weight(i_w){}
        human(){}
        ~human(){
            std::cout << "Destroy " << this->name << " Class" << std::endl;
        }
};


int main()
{
    human *pig = new human;
    human *taemin = new human("Yeon", 28, 80.0);

    std::cout << pig->get_name() << std::endl;
    std::cout << pig->get_age() << std::endl;
    std::cout << pig->get_weight() << std::endl;

    std::cout << taemin->get_name() << std::endl;
    std::cout << taemin->get_age() << std::endl;
    std::cout << taemin->get_weight() << std::endl;

    std::cout << "Program memory exit()" << std::endl;
    delete(pig);        //delete pig;
    delete(taemin);     //delete tamin;

    return 0;
}