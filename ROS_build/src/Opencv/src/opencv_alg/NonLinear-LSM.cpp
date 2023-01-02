#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "stdlib.h"

template <typename T1>
class Plot
{
    public:
        const T1 this_name;
        int circle_size;
        std::vector<cv::Point2f> Point_v;

        void init_val(cv::Mat* init, cv::Mat* convert){
            srand(time(NULL));
            circle_size = rand() % 300;
            std::cout << "circle_size = " << circle_size << ", Center(x,y) = (" << (init->rows/2) << ","<< (init->rows/2) << ")" << std::endl;
            cv::circle(*init, cv::Point(init->rows/2, init->rows/2), circle_size, cv::Scalar(255,0,255), 1, 8, 0);
            Check_Point(init);

            for(int i = 0; i < Point_v.size()/2; i++)
                draw_point(Point_v[i], convert, cv::Scalar(255,255,255));
        }

        inline void draw_point(cv::Point init_pt, cv::Mat* init, cv::Scalar color){
            cv::circle(*init, init_pt, 1, color, -1, 1);
        }
        inline void draw_line(cv::Point rand_pt1, cv::Point rand_pt2, cv::Mat* init, cv::Scalar color){
            cv::line(*init, rand_pt1, rand_pt2, color, 1);
        }
        void Check_Point(cv::Mat* input){
            cv::Point point(0,0);
            for(int i = 0; i < input->rows; i++)
                for(int j = 0; j < input->cols; j++){
                    int r = input->at<cv::Vec3b>(i, j)[0];
                    int g = input->at<cv::Vec3b>(i, j)[1];
                    int b = input->at<cv::Vec3b>(i, j)[2];

                    if(r == 255 && g == 0 && b == 255)
                        Point_v.push_back(cv::Point(i,j+100));
                }
            std::cout << "Point = " << Point_v.size() << std::endl;
        }

        Plot(){}
        Plot(std::string name):this_name(name){}
        Plot(std::string name, cv::Mat* init, cv::Mat* convert):this_name(name){
            init_val(init, convert);
        }
        ~Plot(){
            std::cout << "Destroy [" << this_name << "] " << std::endl;
        }
};

template <typename Param1, typename Param2, typename Param3>
class Solver : public Plot<std::string>
{
    double error = 0.0001;
    Plot<std::string> pt;

    public:     
        double function(cv::Point Point, Param1 input_a, Param2 input_b, Param3 input_r){  //(x-a)^2 + (y-b)^2 = r^2
            return sqrt(pow(Point.x-input_a, 2) + pow(Point.y-input_b, 2)) - input_r;
        }
        void D_function(std::vector<cv::Point2f>& Point_v, Param1 input_a, Param2 input_b, Param3 input_r, cv::Mat* visual){
            int v_size = Point_v.size();
            // std::cout << "Point_v size = " << v_size << ", a=" << input_a << " b=" << input_b << " r=" << input_r << std::endl;
            cv::Mat input_x = cv::Mat::zeros(3, 1, CV_32FC1);
            input_x.at<float>(0,0) = (float)input_a;
            input_x.at<float>(1,0) = (float)input_b;
            input_x.at<float>(2,0) = (float)input_r;

            cv::Mat X = cv::Mat::zeros(3, 1, CV_32FC1);  //a,b,r Parameter
            cv::Mat Y = cv::Mat::zeros(v_size, 1, CV_32FC1);  //function Metrix
            cv::Mat Jacobian = cv::Mat::zeros(v_size, 3, CV_32FC1);   //F`

            for(int k = 0; k < 1000; k++){
                cv::Mat input = cv::Mat::zeros(800, 800, CV_8UC3);
                input += *visual;

                for(int i = 0; i < v_size; i++){
                    Jacobian.at<float>(i,0) = 2 * Point_v[i].x;
                    Jacobian.at<float>(i,1) = 2 * Point_v[i].y;
                    Jacobian.at<float>(i,2) = -1.0f;

                    Y.at<float>(i,0) = function(Point_v[i], input_x.at<float>(0,0), input_x.at<float>(1,0), input_x.at<float>(2,0));
                    // Y.at<float>(i,0) = -pow(input_a, 2) - pow(input_b, 2);
                }
                // X = ((Jacobian.t() * Jacobian).inv()) * Jacobian.t() * Y;
                X = Jacobian.inv(cv::DECOMP_SVD) * Y;
                input_x.at<float>(0,0) -= X.at<float>(0,0);
                input_x.at<float>(1,0) -= X.at<float>(1,0);
                input_x.at<float>(2,0) -= X.at<float>(2,0);

                std::cout << input_x << std::endl;
                cv::Point pt;
                pt.x = input_x.at<float>(0,0);
                pt.y = input_x.at<float>(1,0);
                int radi = fabs(input_x.at<float>(2,0));
                cv::circle(input, pt, radi, cv::Scalar(0,255,0), 1, 8);
                cv::imshow("Convert", input);
            }
        }

        Solver(){}
        Solver(std::vector<cv::Point2f>& Points, Param1 input_a, Param2 input_b, Param3 input_r, cv::Mat* visual){
            D_function(Points, input_a, input_b, input_r, visual);
        }
        ~Solver(){}
};

int main()
{
    cv::Mat* init_mat = new cv::Mat(800, 800, CV_8UC3);
    cv::Mat* convert_mat = new cv::Mat(800, 800, CV_8UC3);

    Plot<std::string> pt("init", init_mat, convert_mat);

    while(1){
        Solver<double, double, double> sv(pt.Point_v, 400.0, 400.0, 100.0, convert_mat);

        if(!init_mat->empty())
            cv::imshow("Plot", *init_mat);
        // if(!convert_mat->empty())
        //     cv::imshow("Convert", *convert_mat);

        cv::waitKey(1);
    }


    // delete(init_mat);
    // pt.~Plot();
    return 0;
}