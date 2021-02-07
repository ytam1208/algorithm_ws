#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

double gaussianRandom(void)
{
    double v1, v2, s;

    do
    {
        v1 = 2 * ((double)rand() / RAND_MAX) - 1; // -1.0 ~ 1.0 까지의 값
        v2 = 2 * ((double)rand() / RAND_MAX) - 1; // -1.0 ~ 1.0 까지의 값
        s = v1 * v1 + v2 * v2;
    } while (s >= 1 || s == 0);

    s = sqrt((-2 * log(s)) / s);

    return v1 * s;
}

double makegaussianRandom(double average, double segma)
{
    srand(cv::getTickCount());
    double R1;
    double result;

    int max_random_value = 2;
    int Rev_max_random_value = max_random_value - 1;
    R1 = max_random_value * ((double)rand() / RAND_MAX) - Rev_max_random_value;

    double front =  1/(segma * sqrt(2 * M_PI));
    double back = -pow((R1 - average),2) / (2 * pow(segma, 2));

    result = front * exp(back);
    std::cout << "result = " << result << std::endl;
}

int main(void)
{

    cv::Mat src = cv::Mat::zeros(500, 500, CV_8U);

    while (1)
    {
        makegaussianRandom(0, 0.1);
        //cv::imshow("src", src);
        //cv::waitKey(1);
    }
    return 0;
}
