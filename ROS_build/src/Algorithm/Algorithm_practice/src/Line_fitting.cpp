#include <iostream>
#include <random>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>

class Check
{
    public:
        bool operator()(bool flag, std::string f_name){
            if(!flag){
                std::cout << f_name << " Error!" << std::endl;
                return false;
            }
            return true;
        }
}SYSTEM;


class Plot
{
    private:
        cv::Point Model_first = cv::Point(300, 500);
        cv::Point Model_last = cv::Point(700, 500);
    public:
        void initalize_Grid(cv::Mat* init){
            cv::line(*init, Model_first, Model_last, cv::Scalar(0,255,0), 2);
        }
        void Make_Random(cv::Mat* init){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0,999);

            std::vector<cv::Point> random_v;
            cv::Point pt;
            for(int i = 0; i < 2; i++){
                pt.x = dis(gen);
                for(int j = 0; j < 2; j++)
                    pt.y = dis(gen);
                random_v.push_back(pt);
            }
            cv::line(*init, random_v[0], random_v[1], cv::Scalar(0,0,255), 2);
            initalize_Grid(init);    
        }
        void Visualize(cv::Mat* Plot){
            cv::imshow("Plot", *Plot);
            cv::waitKey(1);
        }
    public: 
        bool operator()(cv::Mat* init){
            if(!init->empty()){
                Visualize(init);
            }
            else
                return false;
            return true;
        }
        Plot(){}
        ~Plot(){}
}PLOT;


int main(int argc, char** argv)
{
    cv::Mat* Plot = new cv::Mat(1000, 1000, CV_8UC3);
    PLOT.Make_Random(Plot);
    while(1){
        SYSTEM(PLOT(Plot), "Plot");

    }
    return 0;
}
