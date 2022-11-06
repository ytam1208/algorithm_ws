#include "Opencv/Opencv_tuto.hpp"

int main()
{
    cv::Mat src = cv::imread("~/Desktop/test_img.jpg", cv::IMREAD_COLOR);
    
    cv::imshow("color_img", src);

    cv::waitKey(0);
    return 0;
}