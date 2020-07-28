#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

int main()
{
    cv::Mat src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/lena.jpg", cv::IMREAD_COLOR);

    cv::imshow("color_img", src);
    cv::waitKey(0);
    

    return 0;
}