#include <iostream>
#include <random>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
// #include <typeinfo>

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
        std::vector<cv::Point> Init_v;
        std::vector<cv::Point> Rand_v;
    public:
        void initalize_Grid(cv::Mat* init){
            cv::line(*init, Model_first, Model_last, cv::Scalar(0,255,0), 2);
        }
        std::vector<cv::Point> Make_Random(cv::Mat* init){
            SYSTEM(this->Get_Points(init, Init_v), "Get_Init_v_Points");

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0,999);

            std::vector<cv::Point> random_v;
            cv::Point pt;
            cv::Mat rand_m = cv::Mat::zeros(1000, 1000, CV_8UC3);
            for(int i = 0; i < 2; i++){
                pt.x = dis(gen);
                for(int j = 0; j < 2; j++)
                    pt.y = dis(gen);
                random_v.push_back(pt);
            }
            cv::line(rand_m, random_v.at(0), random_v.at(1), cv::Scalar(0,0,255), 2);
            cv::line(*init, random_v.at(0), random_v.at(1), cv::Scalar(0,0,255), 2);

            random_v.clear();
            SYSTEM(this->Get_Points(&rand_m, random_v), "Get_rand_m_Points");
            return random_v;
        }
        bool Get_Points(cv::Mat* init, std::vector<cv::Point>& input_v){
            if(init->empty()) return false;

            for(int i = 0; i < init->rows; i++)
                for(int j = 0; j < init->cols; j++){
                    int b = init->at<cv::Vec3b>(i, j)[0];
                    int g = init->at<cv::Vec3b>(i, j)[1];
                    int r = init->at<cv::Vec3b>(i, j)[2];

                    if(b > 0 || g > 0 || r > 0)
                        input_v.push_back(cv::Point(i,j));
                }
            return true;
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

template <typename T1>
class Solver
{
    cv::Mat Get_Slope(std::vector<T1>& input_v){
        int v_size = input_v.size();
        cv::Mat X = cv::Mat(v_size, 2, CV_32FC1);
        cv::Mat Y = cv::Mat(v_size, 1, CV_32FC1);
        cv::Mat Model = cv::Mat::zeros(2,1, CV_32FC1);

        int i = 0;
        for(auto iter : input_v){
            X.at<float>(i, 0) = iter.x;
            X.at<float>(i, 1) = 1.0;
            Y.at<float>(i) = iter.y;
            i++;
        }
        Model = X.inv(cv::DECOMP_SVD) * Y;
        std::cout << Model << std::endl;
    }
    public:
        bool operator()(std::vector<T1>& input){
            if(input.size() == 0)
                return false;
            Get_Slope(input);
            return true;
        }
    public:
        Solver(){}
        ~Solver(){}
};
Solver<cv::Point> SOLVER;

int main(int argc, char** argv)
{
    cv::Mat* Plot = new cv::Mat(1000, 1000, CV_8UC3);
    PLOT.initalize_Grid(Plot);
    PLOT.Rand_v = std::move(PLOT.Make_Random(Plot));
    std::cout << PLOT.Rand_v.size() << std::endl;
    std::cout << PLOT.Init_v.size() << std::endl;

    while(1){
        // SYSTEM(SOLVER(PLOT.Rand_v), "SOLVER");
        SYSTEM(PLOT(Plot), "Plot");
    }
    return 0;
}
