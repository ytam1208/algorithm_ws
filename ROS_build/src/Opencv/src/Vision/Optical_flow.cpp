#include <iostream>
#include <numeric>
#include <Opencv/Opencv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d.hpp>
#include <Eigen/Core>
#include <pangolin/pangolin.h>
#include <unistd.h>
#include <opencv2/core/fast_math.hpp>

// https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
// std::string t1 = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/L.png";
// std::string t2 = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/R.png";
std::string t1 = "/home/cona/git/algorithm_ws/ROS_build/color/left.png";
std::string t2 = "/home/cona/git/algorithm_ws/ROS_build/color/right.png";

class Corner_Detection
{
  private:  
    cv::Mat t1_img;
    cv::Mat t2_img;

    void Fast_Corner(){
        std::vector<cv::KeyPoint> key_point;
        cv::FAST(t1_img, key_point, 60, true);

        cv::Mat dst = t1_img.clone();
        for(cv::KeyPoint kp : key_point){
            cv::circle(dst, kp.pt, 5, cv::Scalar(0,0,255), 2);
        }
        cv::imshow("src", t1_img);
        cv::imshow("dst", dst);
        cv::waitKey(0);
    };
  public:
    Corner_Detection(cv::Mat &input1, cv::Mat &input2){
        cv::cvtColor(input1, t1_img, cv::COLOR_RGB2GRAY);
        cv::cvtColor(input2, t2_img, cv::COLOR_RGB2GRAY);
        Fast_Corner();
    };  
    ~Corner_Detection(){};
};

int main(int argc, char** argv)
{
    cv::Mat t1_img = cv::imread(t1, cv::IMREAD_COLOR); //GRAYSCALE
    cv::Mat t2_img = cv::imread(t2, cv::IMREAD_COLOR);

    if(t1_img.empty() == true || t2_img.empty() == true)
        return -1;
    
    Corner_Detection(t1_img, t2_img);
    return 0;
}