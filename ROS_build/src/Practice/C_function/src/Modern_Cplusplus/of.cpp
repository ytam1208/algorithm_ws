#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    std::vector<int> data = {2,4,6,8,10,12};
    //all_of : 벡터의 모든 요소가 조건을 만족하는지 확인
    bool allEven = std::all_of(data.begin(), data.end(), [&data](const int& value){return value % 2 == 0;});
    std::cout << "All elements are even: " << (allEven ? "Yes" : "No") << std::endl;

    //any_of : 벡터의 어떤 요소라도 조건을 만족하는지 확인 
    bool anyMultipleOfThree = std::any_of(data.begin(), data.end(), [&data](const int& value){return value % 3 == 0;});
    std::cout << "Any elements is a multiple of three: " << (anyMultipleOfThree ? "Yes" : "No") << std::endl;

    //none_of : 벡터의 모든 요소가 조건을 만족하지 않는지 확인
    bool noneNegative = std::none_of(data.begin(), data.end(), [&data](const int& value){return value < 0;});
    std::cout << "No element are negative: " << (noneNegative ? "Yes" : "No") << std::endl;
    return 0;
}