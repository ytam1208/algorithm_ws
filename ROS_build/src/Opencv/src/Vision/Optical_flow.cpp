#include <iostream>
#include <numeric>
#include <Opencv/Opencv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d.hpp>
#include <unistd.h>
#include <opencv2/core/fast_math.hpp>
#include <opencv2/core/types.hpp>

// https://docs.opencv.org/4.x/d8/d19/tutorial_stitcher.html
// std::string t1 = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/L.png";
// std::string t2 = "/Users/yeontaemin/github/algorithm_ws/ROS_build/color/R.png";
std::string t1 = "/home/cona/git/algorithm_ws/ROS_build/optical/LK1.png";
std::string t2 = "/home/cona/git/algorithm_ws/ROS_build/optical/LK2.png";

class Corner_Detection
{
    private:  
        cv::Mat img;
    public:
        cv::Mat curr_mat, prev_mat;
        cv::Mat key_curr_mat, key_prev_mat;
        std::vector<cv::KeyPoint> currnet_v, prev_v;
    public:
        void init_check(){
            if(currnet_v.size() > 0){
                currnet_v.swap(prev_v);
                currnet_v.clear();
                prev_mat = curr_mat.clone();
            }
        }
        void Fast_Corner(cv::Mat& input){
            curr_mat = input.clone();
            std::vector<cv::KeyPoint> key_point;
            cv::FAST(input, key_point, 60, true);

            cv::Mat dst = input.clone();
            for(cv::KeyPoint kp : key_point){
                cv::circle(dst, kp.pt, 5, cv::Scalar(0,0,255), 2);
            }

            key_curr_mat = dst.clone();
            currnet_v.swap(key_point);

            cv::imshow("Current", input);
            cv::imshow("prev", key_curr_mat);
        };
        bool operator()(cv::Mat& input){
            cv::cvtColor(input, img, cv::COLOR_RGB2GRAY);
            init_check();
            Fast_Corner(img);
        }
        Corner_Detection(){}
        ~Corner_Detection(){};
};

class OpticalFlowTracker
{
    public:
        cv::Mat img1, img2;
        std::vector<cv::KeyPoint> kp1, kp2;
        void calculateOpticalFlow(const cv::Range &range){
            int half_patch_size = 4;
            int iterations = 10;
            for(size_t i = range.start; i < range.end; i++){
                auto kp = kp1[i];
                double dx = 0, dy = 0;
                dx = kp2[i].pt.x - kp.pt.x;
                dy = kp2[i].pt.y - kp.pt.y;

                double cost = 0, lastCost = 0;

            }
        }
        OpticalFlowTracker(cv::Mat& _img1, cv::Mat& _img2, std::vector<cv::KeyPoint>& _kp1, std::vector<cv::KeyPoint>& _kp2) :
        img1(_img1), img2(_img2), kp1(_kp1), kp2(_kp2){}
};

class LK_Optical_flow
{
    private:
        Corner_Detection CD;

        int pyramids = 4;
        double pyramid_scale = 0.5;
        double scales[4] = {1.0, 0.5, 0.25, 0.125};
        std::vector<cv::Mat> pyr1, pyr2;
        cv::Mat mask;
        cv::Mat img;
    public:
        bool Using_opencv(){
            if(CD.prev_mat.empty()){
                return false;
            } 
            else if(!CD.prev_mat.empty() && !CD.curr_mat.empty())
                mask = cv::Mat::zeros(CD.prev_mat.size(), CD.prev_mat.type());

            cv::Mat frame = CD.curr_mat.clone();
            std::vector<uchar> status;
            std::vector<float> err;
            cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT) + (cv::TermCriteria::EPS), 10, 0.03);
            
            std::vector<cv::Point2f> prev, current;
            cv::KeyPoint::convert(CD.prev_v, prev, std::vector<int>());
            cv::KeyPoint::convert(CD.currnet_v, current, std::vector<int>());
            cv::calcOpticalFlowPyrLK(CD.prev_mat, frame, prev, current, status, err, cv::Size(15,15), 2, criteria);
            for(uint i = 0; i < prev.size(); i++)
                if(status[i] == 1){
                    cv::line(mask, current[i], prev[i], cv::Scalar(255), 2);
                    cv::circle(frame, current[i], 2, cv::Scalar(127), -1);
                }
            
            cv::add(frame, mask, img);
            cv::imshow("Frame", img);
            return true;
        }
        void init_check(){
            if(pyr1.size() > 0 || pyr2.size() > 0){
                pyr1.clear();
                pyr2.clear();
            }
        }
        bool Make_pyramids(){
            for(int i = 0; i < pyramids; i++){  
                if(i == 0 && CD.prev_mat.empty())
                    return false;
                if(i == 0){
                    pyr1.push_back(CD.curr_mat);
                    pyr2.push_back(CD.prev_mat);
                }
                else{
                    cv::Mat img1_pyr, img2_pyr;
                    cv::resize(pyr1[i-1], img1_pyr,
                                cv::Size(pyr1[i-1].cols * pyramid_scale, pyr1[i-1].rows * pyramid_scale));
                    cv::resize(pyr2[i-1], img2_pyr,
                                cv::Size(pyr2[i-1].cols * pyramid_scale, pyr2[i-1].rows * pyramid_scale));
                    pyr1.push_back(img1_pyr);
                    pyr2.push_back(img2_pyr);
                }
            }
            for(int i = 0; i < pyr1.size(); i++){
                cv::imshow("current " + std::to_string(i), pyr1[i]);
                cv::imshow("prev " + std::to_string(i), pyr2[i]);
            }
            return true;
        }
        void Coarse_to_fine_pyramids(){
            std::vector<cv::KeyPoint> kp1_pyr, kp2_pyr;
            for(auto &kp : CD.currnet_v){
                auto kp_top = kp;
                kp_top.pt *= scales[pyramids - 1];
                kp1_pyr.push_back(kp_top);
                kp2_pyr.push_back(kp_top);
            }
        }
        void Get_Optical_Flow(){
            for(int level = pyramids - 1; level >= 0; level--){
                
            }
        }
        void Calculate(cv::Mat& img1, cv::Mat& img2, std::vector<cv::KeyPoint>& kp1, std::vector<cv::KeyPoint>& kp2){
            kp2.resize(kp1.size());

        }
        LK_Optical_flow(Corner_Detection &in_CD){
            this->CD = in_CD;
            Using_opencv();
            // init_check();
            // Make_pyramids();
            // Coarse_to_fine_pyramids();
        }
};

int main(int argc, char** argv)
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cout << "Can`t open the Video" << std::endl;
        return -1;
    }

    cv::Mat current_img;
    Corner_Detection cd;

    while(1){
        cap >> current_img;
        if(current_img.empty()){
            std::cout << "empty image" << std::endl;
            return 0;
        }
        cd(current_img);
        LK_Optical_flow lk(cd);
        
        cv::waitKey(1);
        if(cv::waitKey(25)==27)
            break;
    }


    return 0;
}


