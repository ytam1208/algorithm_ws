#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>

template <typename T1>
class Plot
{
    public:
        T1 this_name;
        cv::Point2f rand_pt;
        int rand1, rand2;

        void init_val(cv::Mat* init){
            srand(time(NULL));
            int circle_size = rand() % 500;
            cv::circle(*init, cv::Point(init->rows/2, init->rows/2), circle_size, cv::Scalar(255,255,255), 1, 8, 0);
            
        }

        inline void draw_point(cv::Point init_pt, cv::Mat* init, cv::Scalar color){
            cv::circle(*init, init_pt, 2, color, -1, 1);
        }
        inline void draw_line(cv::Point rand_pt1, cv::Point rand_pt2, cv::Mat* init, cv::Scalar color){
            cv::line(*init, rand_pt1, rand_pt2, color, 1);
        }

        Plot(){}
        Plot(std::string name):this_name(name){}
        Plot(std::string name, cv::Mat* init):this_name(name){
            init_val(init);
        }
        ~Plot(){
            std::cout << "Destroy [" << this_name << "] " << std::endl;
        }
};

int main()
{
    cv::Mat* init_mat = new cv::Mat(800, 800, CV_8UC3);

    Plot<std::string> pt("init", init_mat);

    cv::imshow("Plot", *init_mat);
    cv::waitKey(0);
    return 0;
}