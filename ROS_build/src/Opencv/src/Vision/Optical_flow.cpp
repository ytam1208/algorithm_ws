#include <iostream>
#include <numeric>
#include <Opencv/Opencv.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/features2d.hpp>
#include <Eigen/Core>
#include <pangolin/pangolin.h>
#include <unistd.h>
#include <opencv2/core/fast_math.hpp>

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
            if(currnet_v.size() > 0 || prev_v.size() > 0){
                currnet_v.swap(prev_v);
                currnet_v.clear();
                prev_mat = curr_mat.clone();
            }
        }
        void Fast_Corner(cv::Mat& input){
            std::vector<cv::KeyPoint> key_point;
            cv::FAST(input, key_point, 60, true);

            cv::Mat dst = input.clone();
            for(cv::KeyPoint kp : key_point){
                cv::circle(dst, kp.pt, 5, cv::Scalar(0,0,255), 2);
            }

            curr_mat = input.clone();
            key_curr_mat = dst.clone();
            currnet_v.swap(key_point);
        };

        Corner_Detection(cv::Mat& input){
            cv::cvtColor(input, img, cv::COLOR_RGB2GRAY);
            init_check();
            Fast_Corner(img);
        };
        ~Corner_Detection(){};
};

// class OpticalFlowTracker
// {
//     public:
//         cv::Mat img1, img2;
//         std::vector<cv::KeyPoint> kp1, kp2;
//         void calculateOpticalFlow(const cv::Range &range){

//         }
//         OpticalFlowTracker(cv::Mat& _img1, cv::Mat& _img2, std::vector<cv::KeyPoint>& _kp1, std::vector<cv::KeyPoint>& _kp2) :
//         img1(_img1), img2(_img2), kp1(_kp1), kp2(_kp2){}
// };

// class LK_Optical_flow
// {
//     private:
//         Corner_Detection CD;

//         int pyramids = 4;
//         double pyramid_scale = 0.5;
//         double scales[4] = {1.0, 0.5, 0.25, 0.125};
//         std::vector<cv::Mat> pyr1, pyr2;

//     public:
//         void Make_pyramids(){
//             for(int i = 0; i < pyramids; i++){  
//                 if(i == 0){
//                     pyr1.push_back(CD.Corner_Mv[0]);
//                     pyr2.push_back(CD.Corner_Mv[1]);
//                 }
//                 else{
//                     cv::Mat img1_pyr, img2_pyr;
//                     cv::resize(pyr1[i-1], img1_pyr,
//                                 cv::Size(pyr1[i-1].cols * pyramid_scale, pyr1[i-1].rows * pyramid_scale));
//                     cv::resize(pyr2[i-1], img2_pyr,
//                                 cv::Size(pyr2[i-1].cols * pyramid_scale, pyr2[i-1].rows * pyramid_scale));
//                     pyr1.push_back(img1_pyr);
//                     pyr2.push_back(img2_pyr);
//                 }
//             }
//         }
//         void Coarse_to_fine_pyramids(){
//             std::vector<cv::KeyPoint> kp1_pyr, kp2_pyr;
//             for(auto &kp : CD.Corner_Kv[0]){
//                 auto kp_top = kp;
//                 kp_top.pt *= scales[pyramids - 1];
//                 kp1_pyr.push_back(kp_top);
//                 kp2_pyr.push_back(kp_top);
//             }
//         }
//         void Get_Optical_Flow(){
//             for(int level = pyramids - 1; level >= 0; level--){
                
//             }
//         }
//         void Calculate(cv::Mat& img1, cv::Mat& img2, std::vector<cv::KeyPoint>& kp1, std::vector<cv::KeyPoint>& kp2){
//             kp2.resize(kp1.size());

//         }
//         LK_Optical_flow(Corner_Detection &in_CD){
//             this->CD = in_CD;
//         }
//         void operator()(){
//             Make_pyramids();
//             Coarse_to_fine_pyramids();

//         }
// };

int main(int argc, char** argv)
{
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){
        std::cout << "Can`t open the Video" << std::endl;
        return -1;
    }

    cv::Mat current_img;
    while(1){
        cap >> current_img;
        if(current_img.empty()){
            std::cout << "empty image" << std::endl;
            return 0;
        }
        Corner_Detection cd(current_img);

        cv::waitKey(1);
        if(cv::waitKey(25)==27)
            break;
    }


    // cv::Mat t1_img = cv::imread(t1, cv::IMREAD_COLOR); //GRAYSCALE
    // cv::Mat t2_img = cv::imread(t2, cv::IMREAD_COLOR);

    // if(t1_img.empty() == true || t2_img.empty() == true)
    //     return -1;
    
    // Corner_Detection cd;
    // cd(t1_img);
    // cd(t2_img);

    // LK_Optical_flow lk(cd);
    // lk();

    return 0;
}


