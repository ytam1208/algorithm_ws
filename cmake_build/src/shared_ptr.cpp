#include "system.hpp"
#include <memory>

template <typename T1, typename T2, typename T3>
class human
{
    public:
        T1 name;
        T2 age;
        T3 weight;

        human(){}
        human(T1 i_n, T2 i_a, T3 i_w):name(i_n),age(i_a),weight(i_w){}
        ~human(){
            std::cout << "Destroy class " << this->name << std::endl;
        }
};

template <typename T1, typename T2, typename T3>
class TM : public human<T1, T2, T3>
{
    public:
        T1 T_name;
        T2 T_age;
        T3 T_weight;

        TM(){}
        // TM(T1 i_T, T2 i_a, T3 i_w):
        // T_name(i_T),T_age(i_a),T_weight(i_w){}

        TM(T1 i_T, T2 i_a, T3 i_w):
        human<T1, T2, T3>(i_T, i_a, i_w),T_name(i_T),T_age(i_a),T_weight(i_w){}

        ~TM(){
            std::cout << "Destroy class " << this->T_name << std::endl;
        }
};

int main()
{
    // TM<std::string, int, double> *Tam1 = new TM<std::string, int, double>();
    // std::unique_ptr<TM<std::string, int, double>> point_tm = std::make_unique<TM<std::string, int, double>>();
    // std::unique_ptr<TM<std::string, int, double>> point_tm = std::make_unique<TM<std::string, int, double>>("Tam", 20, 80);

    // 기존 C++11 new를 사용하여 shared_ptr에 원시 값을 가리킬때, 동적할당이 두번 생긴다 TM을 만들어주고 new과정에서
    TM<std::string, int, double> *Tam2 = new TM<std::string, int, double>("Tam", 20, 80);
    std::shared_ptr<TM<std::string, int, double>> new2shart_pt(Tam2);

    // C++14 이후에 나온 make_shared를 사용하여 동적할당이 두번 발생하지 않고 단 한번으로 share tm에 원시 값을 지정해준다.
    std::shared_ptr<TM<std::string, int, double>> make2shart_pt = std::make_shared<TM<std::string, int, double>>("Tm", 20, 80.0);

    return 0;
}