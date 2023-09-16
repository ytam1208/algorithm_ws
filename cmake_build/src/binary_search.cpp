#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
    std::vector<int> Ref{1,3,5,7,9,10};
    std::vector<int> Query{5,7,9,1,2,4};

    for(auto ele : Query){
        std::cout << "Searching for " << ele << std::endl;
        if(std::binary_search(Ref.begin(), Ref.end(), ele)){
            std::cout << "Found " << ele << std::endl;
        } 
        else
            std::cout << "Not Found " << ele << std::endl;
    }
    std::cout << std::endl;

    std::vector<std::vector<int>> matrix;
    matrix.push_back(std::vector<int> {1, 2, 3});
    matrix.push_back(std::vector<int> {4, 5, 6});
    matrix.push_back(std::vector<int> {7, 8, 9});
    for(auto iter : matrix){
        if(std::binary_search(std::begin(matrix), std::end(matrix), std::vector<int>{1},
        [](const std::vector<int>& ref, const std::vector<int>& query){
            return ref[0] < query[0];
        })){
            std::cout << "Found " << iter[0] << std::endl;
        }
        else{
            std::cout << "Not Found " << iter[0] << std::endl;
        }
    }


    return 0;
}