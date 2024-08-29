#include <vector>
#include <iostream>
#include <numeric>
//#include <algorithm>

int main(int argc, char** argv)
{
    std::vector<int> v = {1,2,3,4,5};
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "sum = " << sum << std::endl;
    
    std::vector<int> nums = {3,1,4,1,5,9,2,6,5,3,5};
    int max_value = std::accumulate(nums.begin(), nums.end(), nums[0], 
                                    [](const int& a, const int& b){return std::max(a,b);});
    std::cout << "The maximum value is: " << max_value << std::endl;

    int min_value = std::accumulate(nums.begin(), nums.end(), nums[0],
                                    [](const int& a, const int& b){return std::min(a,b);});
    std::cout << "The minimum value is: " << min_value << std::endl;

    int mean_value = std::accumulate(nums.begin(), nums.end(), 0) / nums.size();
    std::cout << "The mean value is: " << mean_value << std::endl;

    // int scs = std::all_of(nums.begin(), nums.end(), [](const int& value){return value != 1;});
    // std::cout << "There are some not one numbers in the container" << std::endl;

    return 0;
}