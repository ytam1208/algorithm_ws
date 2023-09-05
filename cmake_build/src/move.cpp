#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main()
{
    std::string str = "Hello";
    std::vector<std::string> str_v;

    //push_back(const T&)가 오버로딩 되어서 문자열의 복사가 발생한다.
    str_v.push_back(str);
    
    //push_back(T&&)가 오버로딩 되서 문자열의 복사 없이 그대로 전달된다.
    str_v.push_back(std::move(str));

    //일반적으로 이동된 객체를 접근하는 것은 안되지만 C++ string의 경우 비어있음이 보장된다.
    std::cout << "After move, str is ->" << str << std::endl;
    std::cout << "After contents of the vector are ->" << str_v[0] << ", " << str_v[1] << std::endl; 

    return 0;
}
