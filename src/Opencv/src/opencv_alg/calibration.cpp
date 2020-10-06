#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <ctime>
#include <vector>
#include <thread>

class calibration
{
    public:
        cv::Mat src;        
        cv::Mat gray;

        std::vector<cv::Point3f> objectPoints;            //3d world coordinates
        std::vector<cv::Point2f> imagePoints;             //2d world coordinates

        struct Internal_Cam_Param
        {
            double fx = 496.321719;
            double fy = 495.920276;
            double cx = 314.523890;
            double cy = 236.862583;
            double k1 = 0.110288;
            double k2 = -0.253714;
            double p1 = -0.001977;
            double p2 = -0.000023;
        };
        Internal_Cam_Param Inter_cam;

        bool object_flag;
        int img_rows, img_cols;
        calibration()
        {            
            object_flag = false;
            src = cv::Mat::zeros(480, 640, CV_8UC3);
            img_rows = src.rows;
            img_cols = src.cols;
            
            videocapture();
        }
        ~calibration()
        {

        }
    public:
        std::vector<cv::Point3f> Generate3DPoints(bool &_object_flag, std::vector<cv::Point2f> &_corners);
        int videocapture();
        void SolvePnP(cv::Mat &_src);
};

std::vector<cv::Point3f> calibration::Generate3DPoints(bool &_object_flag, std::vector<cv::Point2f> &_corners)
{
    std::vector<cv::Point3f> points;
    if(_object_flag == false)
    {
        int corner_size = _corners.size();
        for(int i = 0; i < corner_size; i++)
        {
            points.push_back(cv::Point3f(_corners[i].x, _corners[i].y, 30));
            printf("x = %f, y = %f\n", _corners[i].x, _corners[i].y);
        }

        _object_flag = true;
        std::cout << "init_corners" << std::endl;
    }
    return points;
}

int calibration::videocapture()
{
    cv::VideoCapture cap(0);

    if(!cap.isOpened())
    {
        std::cout << "Capture Couldn`t open. " << std::endl;
        return 0;
    }
    
    cv::namedWindow("input_img");
    while(1)
    {
	    cap >> src;
        cv::imshow("input_img", src);
        if(cv::waitKey(1) == 27)
            return 0;
        SolvePnP(src);
    }
}

void calibration::SolvePnP(cv::Mat &_src)
{
    int CHESS_ROWS = 7;
    int CHESS_COLS = 4;
    cv::cvtColor(_src, gray, CV_BGR2GRAY);
    cv::Size chess_size(CHESS_ROWS, CHESS_COLS);
    std::vector<cv::Point2f> corners;
    
    bool found = cv::findChessboardCorners(
        _src,
        chess_size,
        corners,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
        );

    struct tm curr_tm;
    time_t curr_time = time(nullptr);
    localtime_r(&curr_time, &curr_tm);
    int curr_hour = curr_tm.tm_hour;
    int curr_minute = curr_tm.tm_min;
    int curr_second = curr_tm.tm_sec;
    
    if(!found)
    {
        std::cout << " find chessboard Corners failed\n";
        printf("Time = %d:%d:%d\n", curr_hour, curr_minute, curr_second);
    }
    else if(found)
    {
        // cv::cornerSubPix(gray, corners, cv::Size(29, 29), cv::Size(-1, -1),
        // cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        cv::drawChessboardCorners(gray, chess_size, corners, found);

        cv::imshow("gray_img", gray);

        imagePoints = corners;   //2d world coordinates

        if(object_flag == false)
            objectPoints = Generate3DPoints(object_flag, corners);

        else if(object_flag == true)
        {
            //카메라 내부파라미터
            double cam_inParam[9] = {
                Inter_cam.fx, 0, Inter_cam.cx, 0, Inter_cam.fy, Inter_cam.cy, 0, 0, 1
            };
            cv::Mat A(3,3, CV_64FC1, cam_inParam);
            
            //카메라 왜곡계수
            double cam_disCoeffs[4] = { //k1, k2 = 상의 일그러짐, p1, p2 = 접선 왜곡
                Inter_cam.k1, Inter_cam.k2, Inter_cam.p1, Inter_cam.p2
            };
            cv::Mat distCoeffs(4, 1, CV_64FC1, cam_disCoeffs);

            cv::Mat rvec(3, 1, CV_64FC1);
            cv::Mat tvec(3, 1, CV_64FC1); //rotation & translation vectors
            cv::solvePnP(
                objectPoints,
                imagePoints,
                A,
                distCoeffs,
                rvec, tvec
            );

            // std::cout << "rvec: " << rvec << std::endl;
            // std::cout << "tvec: " << tvec << std::endl;
            double a = rvec.at<double>(0,0);
            double b = rvec.at<double>(0,1);
            double c = rvec.at<double>(0,2);

            double tmp = pow(a , 2) + pow(b , 2) + pow(c , 2);
            double degree = sqrt(tmp);
            printf("degree = %lf\n", degree);
            
            cv::Mat R;
            cv::Rodrigues(rvec, R);
            cv::Mat R_inv = R.inv();

            cv::Mat P = -R_inv * tvec;
            double* p = (double *)P.data;

        }
    }

    

    
}

int main(int argc, char **argv)
{
    calibration camera;

    return 0;
}