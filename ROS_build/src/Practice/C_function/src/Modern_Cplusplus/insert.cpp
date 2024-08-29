#include <iostream>
#include <vector>

template <typename T1> 
void Print(std::vector<T1>& data)
{
    std::cout << "Vector after insertion: ";
    for(T1 elem : data)
    {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    std::vector<int> vec = {1,2,4,5};

    vec.insert(vec.begin()+2, 3);
    Print<int>(vec);

    //100을 vec[3] 위치에 3번 삽입
    vec.insert(vec.begin(), 3, 100);
    Print<int>(vec);

    std::vector<int> vec1 = {1, 2, 3};
    std::vector<int> vec2 = {4, 5, 6};
    std::vector<int> vec3 = {7, 8, 9};
    std::vector<int> vec4 = {10, 11, 12};
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
    vec3.insert(vec3.end(), vec4.begin(), vec4.end());
    vec1.insert(vec1.end(), vec3.begin(), vec3.end());
    Print<int>(vec1);

    std::vector<std::string> original = {"apple", "banana", "cherry"};
    std::vector<std::string> target;
    target.insert(target.begin(), std::make_move_iterator(original.begin()), std::make_move_iterator(original.end()));
    Print<std::string>(original);
    Print<std::string>(target);

    std::vector<int> original_int = {1,2,3};
    std::vector<int> target_int;
    target_int.insert(target_int.begin(), std::make_move_iterator(original_int.begin()), std::make_move_iterator(original_int.end()));
    original_int.clear();
    Print<int>(original_int);
    Print<int>(target_int);
}