#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>
#include <ros/ros.h>

class calibration
{
    public:
        cv::Mat src;        

        struct Internal_Cam_Param
        {
            double fx = 496.321719;
            double fy = 495.920276;
            double cx = 314.523890;
            double cy = 236.862583;
            double k1 = 0;
            double k2 = 0;
            double p1 = 0;
            double p2 = 0;
        };
        Internal_Cam_Param Inter_cam;

    
        
        int img_rows, img_cols;
        calibration()
        {            
            src = cv::Mat::zeros(480, 640, CV_8UC3);
            img_rows = src.rows;
            img_cols = src.cols;
            
            videocapture();
        }
        ~calibration()
        {

        }
        int videocapture();
        void SolvePnP();

};