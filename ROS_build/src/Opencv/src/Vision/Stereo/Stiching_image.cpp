#include <iostream>
#include <numeric>
#include <Opencv/Opencv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/stitching.hpp>
#include <Eigen/Core>
#include <pangolin/pangolin.h>
#include <unistd.h>

// https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
// std::string left_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/left.png";
// std::string right_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/right.png";
std::string left_file = "/home/cona/git/algorithm_ws/ROS_build/color/left.png";
std::string right_file = "/home/cona/git/algorithm_ws/ROS_build/color/right.png";

class ORB_feature
{
    private:
        cv::Mat key_L, key_R;
        std::vector<cv::KeyPoint> L_point, R_point;

        void extract_ORB_feature(cv::Mat& left, cv::Mat& right){
            cv::Ptr<cv::Feature2D> orb = cv::ORB::create(); // const static auto&  
            orb->detectAndCompute(left, cv::noArray(), L_point, key_L);
            orb->detectAndCompute(right, cv::noArray(), R_point, key_R);
            matches_ORB_feature(left, right);
        }
        void matches_ORB_feature(cv::Mat& left, cv::Mat& right){
            cv::Mat tmp_left = left;
            cv::Mat tmp_right = right;

            cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
            std::vector<cv::DMatch> matches;
            matcher->match(key_L, key_R, matches);
            std::sort(matches.begin(), matches.end());
            std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + 10);

            cv::Mat Good_dst, dst;
            cv::drawMatches(left, L_point, 
                            right, R_point, 
                            matches, dst, 
                            cv::Scalar::all(-1), cv::Scalar(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            cv::drawMatches(left, L_point, 
                            right, R_point, 
                            good_matches, Good_dst, 
                            cv::Scalar::all(-1), cv::Scalar(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            
            cv::drawKeypoints(left, L_point, left, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
            cv::drawKeypoints(right, R_point, right, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            // cv::imshow("left", left);
            // cv::imshow("right", right);
            // cv::imshow("Matches", dst);
            cv::imshow("Good_Matches", Good_dst);
            cv::waitKey(0);
        }
    public:
        ORB_feature(cv::Mat& left, cv::Mat& right){
            extract_ORB_feature(left, right);
        }
        ~ORB_feature(){}
};

class Stiching
{
    public:
        bool runloop(cv::Mat* left, cv::Mat* right){
            cv::imshow("left", *left);
            cv::imshow("right", *right);
            cv::waitKey(0);

            std::vector<cv::Mat> Mat_v;
            Mat_v.push_back(left->clone());
            Mat_v.push_back(right->clone());

            cv::Mat pano;
            cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
            cv::Ptr<cv::Stitcher> sti = cv::Stitcher::create(mode);
            cv::Stitcher::Status status = sti->stitch(Mat_v, pano);
            if(status != cv::Stitcher::OK){
                std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
                return false;
            }
            cv::imshow("Panorama", pano);
            cv::waitKey(0);
            return true;
        }
        Stiching(cv::Mat* left, cv::Mat* right){
            runloop(left, right);
        }
        ~Stiching(){}
};

int main(int argc, char** argv)
{
    cv::Mat left = cv::imread(left_file, cv::IMREAD_COLOR);
    cv::Mat right = cv::imread(right_file, cv::IMREAD_COLOR);

    if(left.empty() == true || right.empty() == true)
        return -1;
    
    Stiching st(&left, &right);
    ORB_feature ob(left, right);
    return 0;
}