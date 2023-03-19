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
std::string left_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/Taiwan_L.png";
std::string right_file = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/Taiwan_R.png";
// std::string left_file = "/home/cona/git/algorithm_ws/ROS_build/color/left.png";
// std::string right_file = "/home/cona/git/algorithm_ws/ROS_build/color/right.png";

class ORB_feature
{
    private:
        cv::Mat key_L, key_R;
        std::vector<cv::KeyPoint> L_point, R_point;

        cv::Mat Good_dst, dst;

        void extract_ORB_feature(cv::Mat& left, cv::Mat& right){
            cv::Ptr<cv::Feature2D> orb = cv::ORB::create(); // const static auto&  
            orb->detectAndCompute(left, cv::noArray(), L_point, key_L);
            orb->detectAndCompute(right, cv::noArray(), R_point, key_R);
        }
        std::vector<cv::DMatch> matches_ORB_feature(cv::Mat& left, cv::Mat& right){
            cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
            std::vector<cv::DMatch> matches;
            matcher->match(key_L, key_R, matches);
            std::sort(matches.begin(), matches.end());
            std::vector<cv::DMatch> good_matches(matches.begin(), matches.end());

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

            return good_matches;
        }
        cv::Mat Get_homography_matrix(std::vector<cv::KeyPoint>& L_p, std::vector<cv::KeyPoint>& R_p, std::vector<cv::DMatch>& g_m){
            std::vector<int> points1, points2;
            for(auto iter = g_m.begin(); iter != g_m.end(); ++iter){
                points1.push_back(iter->queryIdx);
                points2.push_back(iter->trainIdx);
            }
            std::vector<cv::Point2f> selPoints1, selPoints2;
            cv::KeyPoint::convert(L_p, selPoints1, points1);
            cv::KeyPoint::convert(R_p, selPoints2, points2);            
            cv::Mat h_matrix = findHomography(cv::Mat(selPoints1), cv::Mat(selPoints2), cv::RANSAC, 3 );
            std::cout << "H \n" << h_matrix << std::endl;
            return h_matrix;
        }
        void Panorama_img(cv::Mat& H_Matrix, cv::Mat& left, cv::Mat& right){
            cv::imshow("Origin_r", right);
            cv::imshow("Origin_l", left);
            cv::Mat result_right;
            warpPerspective(right, result_right, H_Matrix, cv::Size(right.cols*2.0, right.rows*1.2), cv::INTER_CUBIC);
            cv::imshow("wrap", result_right);

            cv::Mat matPanorama;
	        matPanorama = result_right.clone(); //복사본 대입
            std::cout << left.cols << ", " << left.rows << std::endl;
	        cv::Mat matROI(matPanorama, cv::Rect(0, 0, left.cols, left.rows));
            cv::imshow("matPanorama", matPanorama);
	        left.copyTo(matROI);
            cv::imshow("copy_matPanorama", matPanorama);

            // cv::imshow("matROI", matROI);
            // cv::imshow("left_matPanorama", left);

            cv::waitKey(0);

        }
        void Display_All(){
            cv::imshow("Matches", dst);
            cv::imshow("Good_Matches", Good_dst);
            cv::waitKey(0);
        }
    public:
        ORB_feature(cv::Mat& left, cv::Mat& right){
            cv::Mat tmp_left = left.clone();
            cv::Mat tmp_right = right.clone();

            extract_ORB_feature(tmp_left, tmp_right);
            std::vector<cv::DMatch> Good_Matches = matches_ORB_feature(tmp_left, tmp_right);
            cv::Mat H = Get_homography_matrix(L_point, R_point, Good_Matches);
            Panorama_img(H, left, right);
            Display_All();
        }
        ~ORB_feature(){}
};

// class Stiching
// {
//     public:
//         bool cv_Stitcher(cv::Mat* left, cv::Mat* right){
//             cv::imshow("left", *left);
//             cv::imshow("right", *right);
//             cv::waitKey(0);

//             std::vector<cv::Mat> Mat_v;
//             Mat_v.push_back(left->clone());
//             Mat_v.push_back(right->clone());

//             cv::Mat pano;
//             cv::Stitcher::Mode mode = cv::Stitcher::PANORAMA;
//             cv::Ptr<cv::Stitcher> sti = cv::Stitcher::create(mode);
//             cv::Stitcher::Status status = sti->stitch(Mat_v, pano);
//             if(status != cv::Stitcher::OK){
//                 std::cout << "Can't stitch images, error code = " << int(status) << std::endl;
//                 return false;
//             }
//             cv::imshow("Panorama", pano);
//             cv::waitKey(0);
//             return true;
//         }
//         Stiching(cv::Mat* left, cv::Mat* right){
//             cv_Stitcher(left, right);
//         }
//         ~Stiching(){}
// };

int main(int argc, char** argv)
{
    cv::Mat left = cv::imread(left_file, cv::IMREAD_COLOR); //GRAYSCALE
    cv::Mat right = cv::imread(right_file, cv::IMREAD_COLOR);

    if(left.empty() == true || right.empty() == true)
        return -1;
    
    // Stiching st(&left, &right);
    ORB_feature ob(left, right);
    return 0;
}