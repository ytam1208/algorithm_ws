#include <iostream>
#include <string.h>
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

namespace Display
{
    class Lodar
    {
        public:
            bool operator()(cv::Mat& mat)
            {
                if(mat.empty())
                    return 0;
                else
                    return 1;
            }
    };
}