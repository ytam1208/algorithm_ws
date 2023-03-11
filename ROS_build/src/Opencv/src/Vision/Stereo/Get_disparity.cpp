#include <iostream>
#include <Opencv/Opencv.hpp>
/*
https://github.com/bill2239/stereo_matching/blob/master/stereo_matching.cpp
https://github.com/luosch/stereo-matching/blob/master/StereoMatching.hpp
*/

std::string left_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/tsukuba_l.png";
std::string right_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/tsukuba_r.png";

cv::Mat compare_OpenCV(cv::Mat *base, cv::Mat *target)
{
    cv::Ptr<cv::StereoSGBM> sgbm = cv::StereoSGBM::create(
        0, 96, 9, 8 * 9 * 9, 32 * 9 * 9, 1, 63, 10, 100, 32);    //매개변수
    cv::Mat disparity_sgbm, disparity;
    sgbm->compute(*base, *target, disparity_sgbm);
    disparity_sgbm.convertTo(disparity, CV_32F, 1.0 / 16.0f);
    return disparity;
}

//Sum_of_Squared_Difference
cv::Mat SSD(cv::Mat *base, cv::Mat *target) 
{
    int kernal_size = 3;
    int adjust = 48; //Max disparity Search range
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
                    depth.at<uchar>(row, col) = (uchar)off;
                    // depth.at<uchar>(row, col) = (uchar)(off * (255/adjust));
                }
            }
    }
    cv::normalize(depth, depth, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());    
    return depth;
}

int main(int argc, char** argv)
{
    cv::Mat left = cv::imread(left_file, cv::IMREAD_GRAYSCALE);
    cv::Mat right = cv::imread(right_file, cv::IMREAD_GRAYSCALE);

    cv::Mat SSD_depth = SSD(&left, &right);
    cv::Mat OpenCV_depth = compare_OpenCV(&left, &right);

    cv::imshow("OpenCV_Disparity", OpenCV_depth/48); 
    cv::imshow("My_SSD_Disparity", SSD_depth);
    cv::imshow("left", left);
    cv::imshow("right", right);
    cv::waitKey(0);
    return 0;
}