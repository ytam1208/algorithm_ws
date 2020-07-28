#include <iostream>
#include <string.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

class opencv_img
{
public:
    cv::Mat src;
    cv::Mat dst;

    std::string user_path = "/home/cona/";
    std::string include_path = "algorithm_ws/src/Opencv/data/lena.jpg";
    std::string path = user_path + include_path;

    opencv_img()
    {
        src = cv::imread(path, cv::IMREAD_COLOR);
        runloop();
    }
    ~opencv_img()
    {
    }


private:
    void runloop()
    {
        imshow_check(src);
        // cv::imshow("colorjpg", src);
        // cv::waitKey(0);
    }
    
    void imshow_check(cv::Mat _src)
    {
        if (_src.empty())
        {
            std::cout << "NO image" << std::endl;
        }
    }
};
