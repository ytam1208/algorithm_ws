#include "calibration/calibration.hpp"


int calibration::videocapture()
{
    cv::VideoCapture cap(0);

    if(!cap.isOpened())
    {
        std::cout << "Capture Couldn`t open. " << std::endl;
        return 0;
    }

    cv::namedWindow("Video");
	while(ros::ok())
	{
	    cap >> src;
        if(src.empty())
        {
            std::cout << "Video over!" << std::endl;
            break;
        }
        cv::Mat flip = cv::Mat::zeros(480, 640, CV_8UC3);

        cv::flip(src, flip, 1);
        cv::swap(src, flip);
        
        cv::imshow("Video", src);
        cv::waitKey(1);
    }
}

void calibration::SolvePnP()
{
    int CHESS_ROWS = 640;
    int CHESS_COLS = 480;

    RETURN_:
    std::vector<cv::Point2f> corner;

    cv::Size chess_size(CHESS_ROWS, CHESS_COLS);
    bool found = cv::findChessboardCorners(
        src,
        chess_size,
        corner,
        CV_CALIB_CB_ADAPTIVE_THRESH
        );
    if(!found)
    {
        std::cout << " find chessboard Corners failed\n";
        goto RETURN_;
    }

    cv::Mat gray;
    cv::cvtColor(src, gray, CV_BGR2GRAY);

    std::vector<cv::Point3f> objectPoints;  //3d world coordinates
    std::vector<cv::Point2f> imagePoints;   //2d world coordinates

    //카메라 내부파라미터
    double cam_inParam[9] = {
        Inter_cam.fx, 0, Inter_cam.cx, 0, Inter_cam.fy, Inter_cam.cy, 0, 0, 1
    };   
    //카메라 왜곡계수
    double cam_disCoeffs[4] = {
        Inter_cam.k1, Inter_cam.k2, Inter_cam.p1, Inter_cam.p2
    };

    cv::Mat rvec, tvec; //rotation & translation vectors
    
    // cv::solvePnP(
    //     objectPoints,
    //     imagePoints,
    //     cam_inParam,
    //     cam_disCoeffs,
    //     rvec, tvec
    // );
}