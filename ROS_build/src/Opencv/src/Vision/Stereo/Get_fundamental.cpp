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
            cv::Mat essential = cv::findEssentialMat(cv::Mat(selPoints1), cv::Mat(selPoints2), focal_length, principal_point );
            cv::Mat homography_matrix = findHomography(cv::Mat(selPoints1), cv::Mat(selPoints2), cv::RANSAC, 3 );

            std::cout << "F-Matrix = \n" << fundemental << std::endl; 
    
            // std::cout << "E-Matrix = \n" << essential << std::endl;         
            // std::cout << "H-Matrix = \n" << homography_matrix << std::endl;         
            compute_fundemental(selPoints1, selPoints2);
            
            cv::imshow("left", left);
            cv::imshow("right", right);
            cv::imshow("dst", dst);
            cv::waitKey(0);
        }

        void compute_fundemental(std::vector<cv::Point2f>& left_ps, std::vector<cv::Point2f>& right_ps)
        {
            cv::Mat M = cv::Mat::zeros(8, 9, CV_32FC1);
            for(int i = 0; i < 8; i++){
                cv::Mat curRow =(cv::Mat_<float>(1, 9) <<  left_ps[i].x * right_ps[i].x, 
                                                            left_ps[i].x * right_ps[i].y,
                                                            left_ps[i].x,                                                        
                                                            right_ps[i].x * left_ps[i].y,                                                        
                                                            left_ps[i].y * right_ps[i].y,                                                        
                                                            left_ps[i].y,                                                        
                                                            right_ps[i].x,                                                        
                                                            right_ps[i].y,                                                        
                                                            1.0f);
                curRow.copyTo(M.row(i));
            }
            cv::Mat normMat1 = calculateNormalizationMatrix(left_ps);
            cv::Mat normMat2 = calculateNormalizationMatrix(right_ps);

            cv::Mat S, U, Vt;
            cv::SVD::compute(M, S, U, Vt);
            std::cout << " Vt = \n" << Vt << std::endl;
            cv::Mat F = Vt.row(7).reshape(1, 3);
            // for(int i = 0; i < F.rows; i++){
            //     for(int j = 0; j < F.cols; j++){
            //         F.at<float>(i,j) /= F.at<float>(2, 2);
            //     }
            // }
            std::cout << "F-Matrix = \n" << F << std::endl;      

            cv::Mat FS, FU, FVt;
            cv::SVD::compute(F, FS, FU, FVt);
            FS.ptr<float>(0)[2] = 0.0;
            F = FU * cv::Mat::diag(FS) * FVt;       
            F = normMat2.t() * F * normMat1;
            cv::Mat F_ = F / F.ptr<float>(2)[2];
            std::cout << "My F-Matrix = \n" << F_ << std::endl;      
        }
        cv::Mat calculateNormalizationMatrix(const std::vector<cv::Point2f>& points)
        {
            if (points.empty()) {
                throw std::runtime_error("empty points");
            }

            std::vector<float> xs, ys;
            xs.reserve(points.size());
            ys.reserve(points.size());
            for (const auto& point : points) {
                xs.emplace_back(point.x);
                ys.emplace_back(point.y);
            }

            float xsMean, xsVariance, ysMean, ysVariance;
            std::tie(xsMean, xsVariance) = calculateMeanVariance(xs);
            std::tie(ysMean, ysVariance) = calculateMeanVariance(ys);

            float sx = std::sqrt(2 / (xsVariance + std::numeric_limits<float>::epsilon()));
            float sy = std::sqrt(2 / (ysVariance + std::numeric_limits<float>::epsilon()));

            // clang-format off
            cv::Mat normMatrix = (cv::Mat_<float>(3, 3) << sx , 0.0, -sx * xsMean,
                                                        0.0, sy , -sy * ysMean,
                                                        0.0, 0.0, 1.0
                                );
            // clang-format on

            return normMatrix;
        }
        std::pair<float, float> calculateMeanVariance(const std::vector<float>& xs)
        {
            if (xs.empty()) {
                throw std::runtime_error("empty vector");
            }

            const float mean = std::accumulate(std::begin(xs), std::end(xs), 0.0) / xs.size();
            const float variance = std::accumulate(std::begin(xs), std::end(xs), 0.0,
                                                [mean](float sum, const float elem) {
                                                    const float tmp = elem - mean;
                                                    return sum + tmp * tmp;
                                                }) /
                                xs.size();
            return std::make_pair(mean, variance);
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