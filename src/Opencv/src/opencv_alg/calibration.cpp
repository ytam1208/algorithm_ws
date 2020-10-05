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

        struct Internal_Cam_Param
        {
            float fx = 496.321719;
            float fy = 495.920276;
            float cx = 314.523890;
            float cy = 236.862583;
            float k1 = 0;
            float k2 = 0;
            float p1 = 0;
            float p2 = 0;
        };
        Internal_Cam_Param Inter_cam;

        int img_rows, img_cols;
        bool object_flag;
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
        int videocapture();
        void SolvePnP(cv::Mat &_src);

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
        cv::cornerSubPix(gray, corners, cv::Size(5, 5), cv::Size(-1, -1),
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));

        cv::drawChessboardCorners(gray, chess_size, corners, found);

        cv::imshow("gray_img", gray);

        std::vector<cv::Point3f> objectPoints;            //3d world coordinates
        std::vector<cv::Point2f> imagePoints = corners;   //2d world coordinates
        if(object_flag == false)
        {
            objectPoints.clear();
            int corner_size = corners.size();
            for(int i = 0; i < corner_size; i++)
            {
                objectPoints.push_back(cv::Point3f(corners[i].x, corners[i].y, 0));
                // printf("objectPoints[i].x = %lf, objectPoints[i].y = %lf, objectPoints[i].z = %lf\n",
                //objectPoints[i].x, objectPoints[i].y, objectPoints[i].z);
            }
            // for(int j = 0; j < img_rows; j++)
            //     for(int i = 0; i < img_cols; i++)
            //     {
                    
            //     }
            object_flag = true;
        }
        else if(object_flag == true)
        {
            //카메라 내부파라미터
            float cam_inParam[9] = {
                Inter_cam.fx, 0, Inter_cam.cx, 0, Inter_cam.fy, Inter_cam.cy, 0, 0, 1
            };
            cv::Mat A(3,3, CV_64FC1, cam_inParam);
            
            //카메라 왜곡계수
            float cam_disCoeffs[4] = { //k1, k2 = 상의 일그러짐, p1, p2 = 접선 왜곡
                Inter_cam.k1, Inter_cam.k2, Inter_cam.p1, Inter_cam.p2
            };
            cv::Mat distCoeffs(4, 1, CV_64FC1, cam_disCoeffs);

            cv::Mat rvec, tvec; //rotation & translation vectors

            cv::solvePnP(
                objectPoints,
                imagePoints,
                A,
                distCoeffs,
                rvec, tvec
            );
            cv::Mat R;
            cv::Rodrigues(rvec, R);
            cv::Mat R_inv = R.inv();

            cv::Mat P = -R_inv * tvec;
            double* p = (double *)P.data;

            printf("x = %lf, y = %lf, z = %lf\n", p[0], p[1], p[2]);
        }

    }
    

    
}

int main(int argc, char **argv)
{
    calibration camera;

    return 0;
}