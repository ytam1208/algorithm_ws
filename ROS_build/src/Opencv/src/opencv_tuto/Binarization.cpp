#include "Opencv/OpencvBase.hpp"

int main()
{
    opencv_img img_;

    cv::Mat logo = img_.src2;
    cv::threshold(logo, logo, 150, 255, 0);

    cv::imwrite("logo.bmp", logo);
    
    return 0;
}