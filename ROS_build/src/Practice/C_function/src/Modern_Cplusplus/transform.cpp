#include <algorithm>
#include <vector>
#include <iostream>
#include <eigen3/Eigen/Core>

int main(int argc, char** argv)
{
    std::vector<int> src = {1,2,3,4,5};
    std::vector<int> dst;
    dst.resize(src.size()+2, 0);

    std::transform(src.begin(), src.end(), dst.begin(), [](int x){return pow(x,2);});
    for(int i : dst)
    {
        std::cout << i << std::endl;
    }


    std::vector<Eigen::Vector4d> points;
    Eigen::Vector4d test(0.0, 0.0, 0.0, 1.0);
    points.insert(points.begin(), 10, test);
    for(const Eigen::Vector4d& iter : points)
    {
        std::cout << iter(0) << ", " << iter(1) << ", " << iter(2) << std::endl;
    }


    std::cout << "New transform" << std::endl;
    Eigen::Matrix4d transformation;
    transformation << 0, 0, 0, 1,
                      0, 0, 0, 0,
                      0, 0, 0, 0,
                      0, 0, 0, 1;

    std::transform(points.begin(), points.end(), points.begin(), [&](const auto& point){return transformation * point;});  
    for(const Eigen::Vector4d& iter : points)
    {
        std::cout << iter(0) << ", " << iter(1) << ", " << iter(2) << std::endl;
    }
}