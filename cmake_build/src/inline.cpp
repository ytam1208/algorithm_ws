#include "system.hpp"

// 기존 C언어 문법에서 자주 사용되었던 매크로 함수
#define Double(x) ((x)*(x))

// C++ 언어로 넘어오며 조금 더 가독성이 생긴 인라인 함수
inline int DOUBLE1(int x){
    return x * x;
}

template<typename T> inline T DOUBLE2(T x){
    return x * x;
}

int main()
{
    std::cout << Double(5) << '\n';
    std::cout << DOUBLE1(5) << '\n';
    std::cout << DOUBLE2(5.5) << '\n';

    // 컴파일러 입장에서 기계코드를 작성했을때 생성되는 구조
    // std::cout << (5 * 5) << '\n';
    return 0;
}