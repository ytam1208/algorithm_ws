#include <iostream>
#include <numeric>
#include <Opencv/Opencv.hpp>
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
            std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + 100);

            cv::Mat dst;
            cv::drawMatches(left, L_point, 
                            right, R_point, 
                            good_matches, dst, 
                            cv::Scalar::all(-1), cv::Scalar(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
            
            cv::drawKeypoints(left, L_point, left, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
            cv::drawKeypoints(right, R_point, right, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            std::vector<int> points1;
            std::vector<int> points2;
            for(auto iter = good_matches.begin(); iter != good_matches.end(); ++iter){
                points1.push_back(iter->queryIdx);
                points2.push_back(iter->trainIdx);
            }

            std::vector<cv::Point2f> selPoints1, selPoints2;
            cv::KeyPoint::convert(L_point, selPoints1, points1);
            cv::KeyPoint::convert(R_point, selPoints2, points2);            
            cv::Mat fundemental = cv::findFundamentalMat(cv::Mat(selPoints1), cv::Mat(selPoints2), cv::FM_8POINT);
            
            // 카메라 내부 파라미터 
            double fx = 718.856, fy = 718.856, cx = 607.1928, cy = 185.2157;
            // 베이스 링크
            double b = 0.573;
            double focal_length = 718.856;			
            cv::Point2d principal_point (607.1928, 185.2157);
            // cv::Mat essential = cv::findEssentialMat(cv::Mat(selPoints1), cv::Mat(selPoints2), focal_length, principal_point );
            // cv::Mat homography_matrix = findHomography(cv::Mat(selPoints1), cv::Mat(selPoints2), cv::RANSAC, 3 );

            std::cout << "F-Matrix = \n" << fundemental << std::endl; 
            // std::cout << "E-Matrix = \n" << essential << std::endl;         
            // std::cout << "H-Matrix = \n" << homography_matrix << std::endl;         

            // compute_epilines(fundemental, selPoints1, selPoints2, tmp_left, tmp_right);
            cv::imshow("left", left);
            cv::imshow("right", right);
            cv::imshow("dst", dst);
            cv::waitKey(0);
        }
        void compute_epilines(cv::Mat& fundemental, std::vector<cv::Point2f>& selPoints1, std::vector<cv::Point2f>& selPoints2,
                              cv::Mat& left, cv::Mat& right)
        {
            std::vector<cv::Vec3f> lines1;
            cv::computeCorrespondEpilines(cv::Mat(selPoints1), 1,  fundemental, lines1);
            for (auto iter = lines1.begin(); iter != lines1.end(); ++iter){
                cv::line(right, cv::Point(0, -(*iter)[2] / (*iter)[1]),
                        cv::Point(right.cols, -((*iter)[2] + (*iter)[0] * right.cols) / (*iter)[1]),
                        cv::Scalar(255, 255, 255));
            }            
            std::vector<cv::Vec3f> lines2;
            cv::computeCorrespondEpilines(cv::Mat(selPoints2), 2, fundemental, lines2);
            for (auto iter = lines2.begin(); iter != lines2.end(); ++iter){

                cv::line(left, cv::Point(0, -(*iter)[2] / (*iter)[1]),
                        cv::Point(left.cols, -((*iter)[2] + (*iter)[0] * left.cols) / (*iter)[1]),
                        cv::Scalar(255, 255, 255));
            }            
            cv::imshow("Left Image", left);
            cv::imshow("Right Image", right);          
        }
    public:
        ORB_feature(cv::Mat& left, cv::Mat& right){
            extract_ORB_feature(left, right);
        }
        ~ORB_feature(){}
};

int main(int argc, char** argv)
{
    cv::Mat left = cv::imread(left_file, cv::IMREAD_GRAYSCALE);
    cv::Mat right = cv::imread(right_file, cv::IMREAD_GRAYSCALE);

    ORB_feature ob(left, right);
    return 0;
}