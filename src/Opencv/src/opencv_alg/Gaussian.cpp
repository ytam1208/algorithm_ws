#include <iostream>
#include <math.h>
#include <map>
#include <iomanip>
#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// double gaussianRandom(void)
// {
//     double v1, v2, s;

//     do
//     {
//         v1 = 2 * ((double)rand() / RAND_MAX) - 1; // -1.0 ~ 1.0 까지의 값
//         v2 = 2 * ((double)rand() / RAND_MAX) - 1; // -1.0 ~ 1.0 까지의 값
//         s = v1 * v1 + v2 * v2;
//     } while (s >= 1 || s == 0);

//     s = sqrt((-2 * log(s)) / s);

//     return v1 * s;
// }


double makegaussianRandom()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dist(0, 1);
    std::map<int, int> hist{};

    for(int i = 0; i < 1000; i++)
        ++hist[std::round(dist(gen))];

    for (auto p : hist) {
        std::cout << std::setw(2) << p.first << ' '
                << std::string(p.second / 100, '*') << " " << p.second << '\n';
    }
    
    // std::cout << std::round(dist(gen)) << std::endl;

}


int main(void)
{
    cv::Mat src = cv::Mat::zeros(500, 500, CV_8U);

    // while (1)
    // {
        makegaussianRandom();
        //cv::imshow("src", src);
        //cv::waitKey(1);
    // }
    return 0;
}
