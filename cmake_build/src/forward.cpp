#include <iostream>
#include <memory>
#include <utility>
// #include <type_traits>

class A
{
public:
    A(int&& n){std::cout << "rvalue overload, n=" << n << std::endl;}
    A(int& n){std::cout << "lvalue overload, n=" << n << std::endl;}
};

class B
{
public:
    template<class T1, class T2, class T3>
    B(T1&& t1, T2&& t2, T3&& t3)
        : a1_{std::forward<T1>(t1)},
          a2_{std::forward<T2>(t2)},
          a3_{std::forward<T3>(t3)}{}
private:
    A a1_, a2_, a3_;
};

class C
{
public:
    template<typename T1>
    C(T1&& t1)
        : a{std::forward<T1>(t1)}{std::cout << "My  class a = " << a << std::endl;}
    template<typename T1, typename T2>
    C(T1&& t1, T2&& t2){
        get_r_value(t1, t2);
    }

private:
    int a;
    template<typename T1, typename T2>
    void get_r_value(T1&& input_t1, T2&& input_t2){
        const T1 t1_ = std::forward<T1>(input_t1);
        const T2 t2_ = std::forward<T2>(input_t2);
        std::cout << "get_r_value = " << t1_ << ", " << t2_ << std::endl;
    }

public:
    template<typename T3>
    decltype(auto) move(T3&& input){
        using ReturnType = std::remove_reference_t<T3>&&;
        return static_cast<ReturnType>(input);
    }
};

template <class T, class U>
std::unique_ptr<T> make_unique1(U&& u){
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

template <class T, class... U>
std::unique_ptr<T> make_unique2(U&&... u){
    return std::unique_ptr<T>(new T(std::forward<U>(u)...));
}

int main()
{
    auto p1 = make_unique1<A>(2);
    // std::unique_ptr<A> p2 = make_unique1<A>(2);
    int i = 1;
    auto p3 = make_unique1<A>(i);
    // std::unique_ptr<A> p4 = make_unique1<A>(i);

    auto t = make_unique2<B>(2,i,3);

    C my_class(3);
    C my_class2(10,0.5);

    int a = my_class2.move(4);
    std::cout << "a = " << a << std::endl;
    return 0;
}