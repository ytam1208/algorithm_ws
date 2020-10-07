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

        cv::Mat src2;        
        cv::Mat gray2;

        std::vector<cv::Point3f> objectPoints;            //3d world coordinates
        std::vector<cv::Point2f> imagePoints;             //2d world coordinates

        std::vector<cv::Point3f> objectPoints2;            //3d world coordinates
        std::vector<cv::Point2f> imagePoints2;             //2d world coordinates

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

        struct Internal_Cam_Param2
        {
            double fx = 609.309129;
            double fy = 610.630841;
            double cx = 308.559136;
            double cy = 233.357698;
            double k1 = 0.103587;
            double k2 = -0.192626;
            double p1 = -0.001186;
            double p2 = -0.001881;
        };
        Internal_Cam_Param2 Inter_cam2;

        bool object_flag;
        bool object_flag2;

        int img_rows, img_cols;
        calibration()
        {            
            object_flag = false;
            object_flag2 = false;

            src = cv::Mat::zeros(480, 640, CV_8UC3);
            src2 = cv::Mat::zeros(480, 640, CV_8UC3);

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
        void CameraAngle(cv::Mat &_R_inv, double _px, double _pz);
        void SolvePnP(cv::Mat &_src, cv::Mat &_src2);
        void timecheck();
};

void calibration::CameraAngle(cv::Mat &_R_inv, double _px, double _pz)
{

    double unit_z[] = {0, 0, _pz};
    cv::Mat Zc(3, 1, CV_64FC1, unit_z);
    cv::Mat Zw = _R_inv * Zc;
    double* zw = (double *)Zw.data;

    double pan = atan2(zw[1], zw[0]) - CV_PI/2; //yaw
    double tilt = atan2(zw[2], sqrt(zw[0] * zw[0] + zw[1] * zw[1])); //pitch
    
    double unit_x[] = {_px, 0, 0};
    cv::Mat Xc(3, 1, CV_64FC1, unit_x);
    cv::Mat Xw = _R_inv * Xc;
    double* xw = (double *)Xw.data;
    double xpan[] = {cos(pan), sin(pan), 0};

    double roll = acos(xw[0]*xpan[0]+xw[1]*xpan[1]+xw[2]*xpan[2]);
    if(xw[2]<0) 
        roll = -roll;
    // double RtoD = 180/CV_PI;

    printf("[camera angle] yaw = %lf, pitch = %lf, roll = %lf \n", pan, tilt, roll);    
    // printf("[camera angle] yaw = %lf, pitch = %lf, roll = %lf \n", pan*RtoD, tilt*RtoD, roll*RtoD);
}
void calibration::timecheck()
{
    struct tm curr_tm;
    time_t curr_time = time(nullptr);
    localtime_r(&curr_time, &curr_tm);
    int curr_hour = curr_tm.tm_hour;
    int curr_minute = curr_tm.tm_min;
    int curr_second = curr_tm.tm_sec;
    
    std::cout << " find chessboard Corners failed\n";
    printf("Time = %d:%d:%d\n", curr_hour, curr_minute, curr_second);
}

std::vector<cv::Point3f> calibration::Generate3DPoints(bool &_object_flag, std::vector<cv::Point2f> &_corners)
{
    std::vector<cv::Point3f> points;
    if(_object_flag == false)
    {
        int corner_size = _corners.size();
        for(int i = 0; i < corner_size; i++)
        {
            points.push_back(cv::Point3f(_corners[i].x, _corners[i].y, 0));
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
    cv::VideoCapture cap2(1);

    if(!cap.isOpened() && !cap2.isOpened())
    {
        std::cout << "Capture Couldn`t open. " << std::endl;
        return 0;
    }
    
    while(1)
    {
	    cap >> src;
	    cap2 >> src2;
    
        // cv::namedWindow("input_img");
        // cv::imshow("input_img", src);
        if(cv::waitKey(1) == 27)
            return 0;
        SolvePnP(src, src2);
    }
}

void calibration::SolvePnP(cv::Mat &_src, cv::Mat &_src2)
{
    int CHESS_ROWS = 7;
    int CHESS_COLS = 4;

    cv::cvtColor(_src, gray, CV_BGR2GRAY);
    cv::cvtColor(_src2, gray2, CV_BGR2GRAY);    
    cv::Size chess_size(CHESS_ROWS, CHESS_COLS);

    std::vector<cv::Point2f> corners;
    std::vector<cv::Point2f> corners2;
    
    bool found = cv::findChessboardCorners(
        _src,
        chess_size,
        corners,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE
        );//이미지, 패턴사이즈, 코너들, 플래그
    bool found2 = cv::findChessboardCorners(
        _src2,
        chess_size,
        corners2,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE
        );//이미지, 패턴사이즈, 코너들, 플래그

    if(!found && !found2)
    {
        timecheck();
    }
    else if(found && found2)
    {
        cv::cornerSubPix(gray, corners, cv::Size(7, 7), cv::Size(-1, -1),
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        //cornerSubPix = 보다 정확한 코너 위치를 알려주기 위해서...
        cv::drawChessboardCorners(_src, chess_size, corners, found);

        cv::cornerSubPix(gray2, corners2, cv::Size(7, 7), cv::Size(-1, -1),
        cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        cv::drawChessboardCorners(_src2, chess_size, corners2, found2);
                                //이미지, 패턴사이즈, 코너들, 플래그

        imagePoints = corners;   //2d world coordinates
        imagePoints2 = corners2;   //2d world coordinates

        if(object_flag == false && object_flag2 == false)
        {
            objectPoints = Generate3DPoints(object_flag, corners);
            objectPoints2 = Generate3DPoints(object_flag2, corners2);
        }
        else if(object_flag == true && object_flag2 == true)
        {
            double cam_inParam[9] = {
                Inter_cam.fx, 0, Inter_cam.cx, 0, Inter_cam.fy, Inter_cam.cy, 0, 0, 1
            }; //카메라 내부파라미터
            cv::Mat A(3,3, CV_64FC1, cam_inParam);
            double cam_disCoeffs[4] = { //k1, k2 = 상의 일그러짐, p1, p2 = 접선 왜곡
                Inter_cam.k1, Inter_cam.k2, Inter_cam.p1, Inter_cam.p2
            };  //카메라 왜곡계수
            cv::Mat distCoeffs(4, 1, CV_64FC1, cam_disCoeffs);
            
            double cam_inParam2[9] = {
                Inter_cam2.fx, 0, Inter_cam2.cx, 0, Inter_cam2.fy, Inter_cam2.cy, 0, 0, 1
            }; //카메라 내부파라미터
            cv::Mat A2(3,3, CV_64FC1, cam_inParam2);
            double cam_disCoeffs2[4] = { //k1, k2 = 상의 일그러짐, p1, p2 = 접선 왜곡
                Inter_cam2.k1, Inter_cam2.k2, Inter_cam2.p1, Inter_cam2.p2
            };  //카메라 왜곡계수
            cv::Mat distCoeffs2(4, 1, CV_64FC1, cam_disCoeffs2);

            cv::Mat rvec(3, 1, CV_64FC1); //rotation & 회전변환
            cv::Mat tvec(3, 1, CV_64FC1); //translation vector & 평행이동

            cv::Mat rvec2(3, 1, CV_64FC1); //rotation & 회전변환
            cv::Mat tvec2(3, 1, CV_64FC1); //translation vector & 평행이동

            cv::solvePnP(
                objectPoints,
                imagePoints,
                A,
                distCoeffs,
                rvec, tvec
            ); //3D 월드좌표를 3D 카메라 좌표로 변환시키는 변환정보(rvec, tvec)를 반환

            cv::Mat R;
            cv::Rodrigues(rvec,R);
            cv::Mat R_inv = R.inv();
            cv::Mat P = -R_inv * tvec;
            double* p = (double *)P.data;

            printf("[camera position] x= %lf, y= %lf, z= %lf \n", p[0], p[1], p[2]); 
            //카메라의 자세(방향) 행렬 R로 추출 가능
            CameraAngle(R_inv, p[0], p[2]);

            cv::solvePnP(
                objectPoints2,
                imagePoints2,
                A2,
                distCoeffs2,
                rvec2, tvec2
            ); //3D 월드좌표를 3D 카메라 좌표로 변환시키는 변환정보(rvec, tvec)를 반환

            cv::Mat R2;
            cv::Rodrigues(rvec2,R2);
            cv::Mat R_inv2 = R2.inv();
            cv::Mat P2 = -R_inv2 * tvec2;
            double* p2 = (double *)P2.data;

            printf("[camera2 position] x2= %lf, y2= %lf, z2= %lf \n", p2[0], p2[1], p2[2]); 
            //카메라의 자세(방향) 행렬 R로 추출 가능
            CameraAngle(R_inv2, p2[0], p2[2]);

            // std::cout << "rvec: " << rvec << std::endl; //Rodrigus 표현이기에 회전변환 R을 추출해야한다.
            // std::cout << "tvec: " << tvec << std::endl;

            // double degree = sqrt(pow(p[0], 2) + pow(p[1], 2) + pow(p[2], 2));
            // printf("camera_angle = %lf \n", degree);
            cv::namedWindow("drawChessboard");
            cv::namedWindow("drawChessboard2");

            cv::line(_src, cv::Point2d(p[0], p[1]), cv::Point2d(p[0], p[1]),cv::Scalar(0,255,0),5,5);
            cv::line(_src, cv::Point2d(320, 240), cv::Point2d(320, 240),cv::Scalar(0,0,255),5,5);

            cv::line(_src2, cv::Point2d(p2[0], p2[1]), cv::Point2d(p2[0], p2[1]),cv::Scalar(0,255,0),5,5);
            cv::line(_src2, cv::Point2d(320, 240), cv::Point2d(320, 240),cv::Scalar(0,0,255),5,5);

            cv::imshow("drawChessboard", _src);
            cv::imshow("drawChessboard2", _src2);

        }
    }
}

int main(int argc, char **argv)
{
    calibration camera;

    return 0;
}