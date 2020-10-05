#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <thread>

class calibration
{
    public:
        cv::Mat src;        
        cv::Mat gray;

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

int calibration::videocapture()
{
    cv::VideoCapture cap(0);

    if(!cap.isOpened())
    {
        std::cout << "Capture Couldn`t open. " << std::endl;
        return 0;
    }

    cv::namedWindow("input_img");
    cv::namedWindow("gray_img");

	while(1)
	{
	    cap >> src;
        if(src.empty())
        {
            std::cout << "Video over!" << std::endl;
            break;
        }
        SolvePnP();

        cv::waitKey(1);
    }
}

void calibration::SolvePnP()
{
    int CHESS_ROWS = 7;
    int CHESS_COLS = 4;

    RETURN_:
    cv::Size chess_size(CHESS_ROWS, CHESS_COLS);
    std::vector<cv::Point2f> corners;

    bool found = cv::findChessboardCorners(
        src,
        chess_size,
        corners,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS
        );
    if(!found)
    {
        std::cout << " find chessboard Corners failed\n";
        goto RETURN_;
    }
    else if(found)
    {
        cv::cvtColor(src, gray, CV_BGR2GRAY);
        cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
         cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
    }

    cv::imshow("input_img", src);
    cv::imshow("gray_img", gray);

    // std::vector<cv::Point3f> objectPoints;  //3d world coordinates
    // std::vector<cv::Point2f> imagePoints;   //2d world coordinates

    // //카메라 내부파라미터
    // double cam_inParam[9] = {
    //     Inter_cam.fx, 0, Inter_cam.cx, 0, Inter_cam.fy, Inter_cam.cy, 0, 0, 1
    // };   
    // //카메라 왜곡계수
    // double cam_disCoeffs[4] = {
    //     Inter_cam.k1, Inter_cam.k2, Inter_cam.p1, Inter_cam.p2
    // };

    // cv::Mat rvec, tvec; //rotation & translation vectors
    
    // cv::solvePnP(
    //     objectPoints,
    //     imagePoints,
    //     cam_inParam,
    //     cam_disCoeffs,
    //     rvec, tvec
    // );
}

int main(int argc, char **argv)
{
    calibration camera;
}