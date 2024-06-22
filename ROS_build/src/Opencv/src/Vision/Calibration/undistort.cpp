#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

/*************************************************** */
std::string window_user = "/home/cona";
std::string mac_user = "/Users/yeontaemin";

std::string folder_path = window_user + "/github/algorithm_ws/ROS_build/src/Opencv/src/Vision/Calibration/chessboard/";

std::string left_img = folder_path + "left12.jpg";
/*************************************************** */

cv::Mat cameraMatrix = (cv::Mat_<double>(3,3) << 5.3591573396163199e+02,                    0.0, 3.4228315473308373e+02, 
                                                                    0.0, 5.3591573396163199e+02, 2.3557082909788173e+02, 
                                                                    0.0,                    0.0,                    1.0);
cv::Mat distCoeffs = (cv::Mat_<double>(5,1) << -2.6637260909660682e-01, 
                                                -3.8588898922304653e-02,
                                                1.7831947042852964e-03, 
                                                -2.8122100441115472e-04,
                                                2.3839153080878486e-01);

cv::Mat UseOpenCV(cv::Mat& image)
{
    cv::Size imageSize = image.size();
    cv::Mat map1, map2;
    cv::initUndistortRectifyMap(cameraMatrix, distCoeffs, cv::Mat(),
                            cv::getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
                            imageSize, CV_32FC1, map1, map2);
    
    cv::Mat undistortedImage;
    cv::remap(image, undistortedImage, map1, map2, cv::INTER_LINEAR);    
    
    cv::Mat combind = cv::Mat::zeros(imageSize.height*2, imageSize.width, CV_16SC3);
    cv::hconcat(image, undistortedImage, combind);

    // cv::imshow("Distorted_img", image);
    // cv::imshow("Undistorted_img", undistortedImage);
    // cv::imshow("result", combind);
    // cv::waitKey(0);
    return combind;
}

cv::Mat Manual(cv::Mat& image)
{
    cv::Mat undistort = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);

    double fx = cameraMatrix.at<double>(0,0);
    double fy = cameraMatrix.at<double>(1,1);
    double cx = cameraMatrix.at<double>(0,2);
    double cy = cameraMatrix.at<double>(1,2);

    double k1 = distCoeffs.at<double>(0,0);
    double k2 = distCoeffs.at<double>(1,0);
    double p1 = distCoeffs.at<double>(2,0);
    double p2 = distCoeffs.at<double>(3,0);
    double k3 = distCoeffs.at<double>(4,0);

    std::cout << "k = " << k1 << ", " << k2 << ", " << k3 << std::endl;
    std::cout << "p = " << p1 << ", " << p2 << std::endl;

    cv::Mat map1 = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    cv::Mat map2 = cv::Mat::zeros(image.rows, image.cols, CV_32F);
    for(int v=0; v < image.rows; v++)
    {
        for(int u=0; u <image.cols; u++)
        {
            double normalize_x = (u-cx) / fx;
            double normalize_y = (v-cy) / fy;
            double r = sqrt(pow(normalize_x,2)+pow(normalize_y,2));
            double rr = pow(r,2);
            double rrrr = pow(r,4);
            double rrrrrr = pow(r, 6);

            double raidal_distortion = 1 + k1*rr + k2*rrrr + k3*rrrrrr;
            double x_tangential_distortion = 2 * p1 * normalize_x * normalize_y + p2 * (rr + 2 * pow(normalize_x, 2));
            double y_tangential_distortion = p1 * (rr + 2 * pow(normalize_y, 2)) + 2 * p2 * normalize_x * normalize_y;

            double normalize_undistorted_x = normalize_x * raidal_distortion + x_tangential_distortion;
            double normalize_undistorted_y = normalize_y * raidal_distortion + y_tangential_distortion;
                                                    
            double undistorted_x = fx * normalize_undistorted_x + cx;  
            double undistorted_y = fy * normalize_undistorted_y + cy;

            if(undistorted_x < 0 || undistorted_y < 0 || undistorted_x > image.cols || undistorted_y > image.rows)
            {
                continue;
            }
            map1.at<float>(v,u) = (float)undistorted_x;
            map2.at<float>(v,u) = (float)undistorted_y;
        }
    }
    cv::Mat undistortedImage;
    cv::remap(image, undistortedImage, map1, map2, cv::INTER_LINEAR);    

    cv::Mat combind = cv::Mat::zeros(image.rows, image.cols*2, CV_8UC1);
    cv::hconcat(image, undistortedImage, combind);

    return combind;
}

int main(int argc, char** argv)
{
    cv::Mat frame = cv::imread(left_img);
    if(frame.empty()==true)
    {
        std::cerr << "no data" << std::endl;
        return 0;
    }
    cv::Mat cv_img = UseOpenCV(frame);
    cv::Mat m_img = Manual(frame);

    while(1)
    {
        cv::imshow("cv_result", cv_img);
        cv::imshow("m_result", m_img);

        char key = cv::waitKey(1);
        if(key == 'q' || key == 'Q' || key == 27){break;}
    }
    return 0;
}