#include <list>
#include <iostream>

int main(int argc, char** argv)
{
    std::list<int> lt; 
    lt.push_back(10);
    lt.push_back(10);
    lt.push_back(108);
    lt.push_back(60);
    lt.push_back(10);
    lt.push_back(100);
    lt.push_back(40);    
    lt.push_back(50);
    lt.push_back(10);
    lt.push_back(109);
    lt.push_back(30);    
    lt.push_back(220);
    lt.push_back(10);  

    // compile error!! .at, [] 없음.
    // for(int i = 0; i < lt.size(); i++)
    //     std::cout << lt[i] << std::endl; 

    std::list<int>::iterator iter;
    for(iter = lt.begin(); iter != lt.end(); iter++)
        std::cout << *iter << ", ";
    std::cout << std::endl;

    for(auto iter : lt)
        std::cout << iter << ", ";
    std::cout << std::endl;

    //unique 멤버 함수를 통해서 붙어있는 인자 삭제.
    lt.unique(); 
    for(auto iter : lt)
        std::cout << iter << ", ";
    std::cout << std::endl;    

    //sort 기본 값은 오름차순으로 정렬.
    lt.sort();
    for(auto iter : lt)
        std::cout << iter << ", ";
    std::cout << std::endl;        

    //std::greater<>() 파라미터를 붙여 sort 기본 값은 내림차순으로 정렬.
    lt.unique(); 
    lt.sort(std::greater<>());
    for(auto iter : lt)
        std::cout << iter << ", ";
    std::cout << std::endl;        

    std::list<float> lt2 = {1.1, 1.2, 2.3, 2.3, 1.2, 3.6, 2.7, 4.8, 5.5, 6.1}; 
    for(auto iter : lt2)
        std::cout << iter << ", ";
    std::cout << std::endl;        
    
    lt2.sort();
    lt2.unique();
    for(auto iter : lt2)
        std::cout << iter << ", ";
    std::cout << std::endl;        


    return 0;
}