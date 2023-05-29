#include <iostream>
#include <Opencv/Opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ximgproc.hpp>

#include <Eigen/Core>
#include <pangolin/pangolin.h>
#include <unistd.h>
/*
https://github.com/bill2239/stereo_matching/blob/master/stereo_matching.cpp
https://github.com/luosch/stereo-matching/blob/master/StereoMatching.hpp
*/

// std::string left_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/left.png";
// std::string right_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/right.png";
std::string left_file = "/home/cona/github/algorithm_ws/ROS_build/color/left.png";
std::string right_file = "/home/cona/github/algorithm_ws/ROS_build/color/right.png";

cv::Mat compare_OpenCV(cv::Mat *base, cv::Mat *target)
{
    cv::Mat left_disparity, disparity, right_disparity, filtered_disparity, showFilteredDisparity;

    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
        0, 96, 9, 8 * 9 * 9, 32 * 9 * 9, 1, 63, 10, 100, 32);    //매개변수

    cv::Ptr<cv::ximgproc::DisparityWLSFilter> wls_filter;
    wls_filter = cv::ximgproc::createDisparityWLSFilter(sgbm);
    cv::Ptr<cv::StereoMatcher> sm = cv::ximgproc::createRightMatcher(sgbm);
    double lambda = 8000.0;
    double sigma = 1.5;

    sgbm->compute(*base, *target, left_disparity);
    cv::normalize(left_disparity, disparity, 0, 255, 32, CV_8U);

    sm->compute(*target, *base, right_disparity);
    wls_filter->setLambda(lambda);
    wls_filter->setSigmaColor(sigma);

    // double filtering_time = (double)cv::getTickCount();
    wls_filter->filter(left_disparity, *base, filtered_disparity, right_disparity);
    // filtering_time = ((double)cv::getTickCount() - filtering_time)/cv::getTickFrequency();
    // std::cout << "Process Time = " << filtering_time << std::endl;

    filtered_disparity.convertTo(showFilteredDisparity, CV_8U, 255 / (96*16.));
    // cv::imshow("L", *base);
    // cv::imshow("R", *target);
    cv::imshow("SGBM_disparity", disparity);
    cv::imshow("filtered_disparity", showFilteredDisparity);
    cv::waitKey(0);

    return showFilteredDisparity;
}

//Sum_of_Squared_Difference
cv::Mat SSD(cv::Mat *base, cv::Mat *target) 
{
    int kernal_size = 9;
    int adjust = 96; //Max disparity Search range
    int height = base->size().height;
    int width = base->size().width;
    std::vector<std::vector<int>> min_ssd;
    cv::Mat depth = cv::Mat(height, width, 0);

    for(int i = 0; i < height; ++i){
        std::vector<int> tmp(width, std::numeric_limits<int>::max());
        min_ssd.push_back(tmp);
    }
    for(int off = 0; off <= adjust; off++){
        // std::cout << "off " << off << std::endl;
        for(int row = 0; row < height; row++)
            for(int col = 0; col < width; col++){
                int start_x = std::max(0, col - kernal_size);
                int start_y = std::max(0, row - kernal_size);
                int end_x = std::min(width-1, col + kernal_size);
                int end_y = std::min(height-1, row + kernal_size);
                int sum_sd = 0;

                for(int i = start_y; i <= end_y; i++)
                    for(int j = start_x; j <= end_x; j++){
                        int delta = abs(base->at<uchar>(i,j) - target->at<uchar>(i,j-off));   //left img 기준으로 right img의 disparity map을 구할 경우
                        //int delta = abs(base->at<uchar>(i,j) - target->at<uchar>(i,j+off)); //right img 기준으로 left img의 disparity map을 구할 경우
              
                        sum_sd += delta * delta;
                    }
                if(sum_sd <= min_ssd[row][col]){
                    min_ssd[row][col] = sum_sd;
                    // depth.at<uchar>(row, col) = (uchar)off;
                    depth.at<uchar>(row, col) = (uchar)(off * (255/adjust));
                }
            }
    }
    return depth;
}


void showPointCloud(const std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> &pointcloud) {

    if (pointcloud.empty()) {
        std::cerr << "Point cloud is empty!" << std::endl;
        return;
    }

    pangolin::CreateWindowAndBind("Point Cloud Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0)
    );

    pangolin::View &d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
        .SetHandler(new pangolin::Handler3D(s_cam));

    while (pangolin::ShouldQuit() == false) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        glPointSize(2);
        glBegin(GL_POINTS);
        for (auto &p: pointcloud) {
            glColor3f(p[3], p[3], p[3]);
            glVertex3d(p[0], p[1], p[2]);
        }
        glEnd();
        pangolin::FinishFrame();
        usleep(1000);   // sleep 5 ms
    }
    return;
}

void comput_3D_point(cv::Mat* base, cv::Mat* depth)
{
    // 카메라 내부 파라미터 
    double fx = 718.856, fy = 718.856, cx = 607.1928, cy = 185.2157;
    // 베이스 링크
    double b = 0.573;

    std::vector<Eigen::Vector4d, Eigen::aligned_allocator<Eigen::Vector4d>> pointcloud;
    for(int v = 0; v < depth->rows; v++)
        for(int u = 0; u < depth->cols; u++){
            // if(depth->at<float>(v,u) <= 0.0 || depth->at<float>(v,u) >= 96.0) continue;

            Eigen::Vector4d point(0, 0, 0, base->at<uchar>(v, u)/255.0); 
            double x = (u - cx) / fx;
            double y = (v - cy) / fy;
            double depth_p = fx * b / (depth->at<float>(v,u));

            point[0] = x * depth_p;
            point[1] = y * depth_p;
            point[2] = depth_p;
            pointcloud.push_back(point);
        }    
        
    // showPointCloud(pointcloud);
}

int main(int argc, char** argv)
{
    cv::Mat left = cv::imread(left_file, cv::IMREAD_GRAYSCALE);
    cv::Mat right = cv::imread(right_file, cv::IMREAD_GRAYSCALE);

    cv::Mat SSD_depth = SSD(&left, &right);
    cv::Mat OpenCV_depth = compare_OpenCV(&left, &right);
    comput_3D_point(&left, &SSD_depth);
    cv::normalize(SSD_depth, SSD_depth, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());    

    cv::imshow("OpenCV_Disparity", OpenCV_depth); 
    cv::imshow("My_SSD_Disparity", SSD_depth);
    cv::imshow("left", left);
    cv::imshow("right", right);
    cv::waitKey(0);

    return 0;
}