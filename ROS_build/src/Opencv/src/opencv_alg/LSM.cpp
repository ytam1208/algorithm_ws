/*
    Least Mean Square Study [22.12.27]

    White Line = init random line
    Green Line = Least Mean Square line
    
    Reference site
    [0] https://darkpgmr.tistory.com/56
    [1] https://teddy0315.tistory.com/3
    [2] https://mhyun.tistory.com/90
*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
        
inline bool compare(cv::Point2d first, cv::Point2d second){
    return ((first.x > second.x) && (first.y > second.y));
}

template <typename T1>
class Plot
{
    public:
        T1 this_name;
        std::vector<cv::Point2f> rand_pt_v;
        cv::Point2f rand_pt;
        int rand1, rand2;

        void init_val(cv::Mat* init){
            srand(time(NULL));

            for(int i = 0; i < 50; i++){
                rand_pt.x = rand() % 800;
                rand_pt.y = rand() % 300 + 300;
                rand_pt_v.push_back(rand_pt);
                // std::cout << "rand = " << rand_pt.x << ", " << rand_pt.y << std::endl;
            }

            for(auto iter : rand_pt_v)
                draw_point(iter, init, cv::Scalar(0,0,255));

            rand1 = rand() % rand_pt_v.size();
            rand2 = rand() % rand_pt_v.size();

            draw_line(rand_pt_v[rand1], rand_pt_v[rand2], init, cv::Scalar(255,255,255));
        }

        inline void draw_point(cv::Point init_pt, cv::Mat* init, cv::Scalar color){
            cv::circle(*init, init_pt, 2, color, -1, 1);
        }
        inline void draw_line(cv::Point rand_pt1, cv::Point rand_pt2, cv::Mat* init, cv::Scalar color){
            cv::line(*init, rand_pt1, rand_pt2, color, 2);
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

class Solver
{
    public:
        cv::Point2d Mean_pt = cv::Point2d(0.0, 0.0);
        Plot<std::string> new_plot;

        void Summation(){
            for(auto iter : new_plot.rand_pt_v){
                // std::cout << "rand = " << iter.x << ", " << iter.y << std::endl;
                Mean_pt.x += iter.x;
                Mean_pt.y += iter.y;
            }
            Mean_pt.x /= new_plot.rand_pt_v.size();
            Mean_pt.y /= new_plot.rand_pt_v.size();
            // std::cout << "Mean = " << Mean_pt.x << ", " << Mean_pt.y << std::endl;
        }

        void compute_slope_a(cv::Mat* draw){
            double slope_a = 0.0;
            double top = 0.0;
            double bottom = 0.0;
            for(auto iter : new_plot.rand_pt_v){
                top += ((iter.x - Mean_pt.x) * (iter.y - Mean_pt.y));
                bottom += pow((iter.x - Mean_pt.x), 2);
            }
            std::cout << "top = " << top << " bottom = " << bottom << std::endl;
            slope_a = top / bottom;
            double y = Mean_pt.y - Mean_pt.x * slope_a;
            std::cout << "Slope = " << slope_a << " b = " << y << std::endl;

            find_Optimize_line(slope_a, y, draw);
        }

        void find_Optimize_line(double slope, double y, cv::Mat* draw){
            cv::Point2d Op_pt;
            std::vector<cv::Point2d> Optimize_pt;
            for(auto iter : new_plot.rand_pt_v){
                Op_pt.x = iter.x;
                Op_pt.y = slope * iter.x + y;
                // std::cout << "Op = " << Op_pt.x << ", " << Op_pt.y << std::endl;
                new_plot.draw_point(Op_pt, draw, cv::Scalar(0,255,0));
                Optimize_pt.push_back(Op_pt);
            }

            std::sort(Optimize_pt.begin(), Optimize_pt.end(), compare);
            new_plot.draw_line(Optimize_pt.front(), Optimize_pt.back(), draw, cv::Scalar(255,0,0));
        }   

        Solver(){}
        Solver(Plot<std::string> init, cv::Mat* draw){
            new_plot.this_name = "Solver_plot";
            new_plot.rand_pt_v = init.rand_pt_v;
            Summation();
            compute_slope_a(draw);
        }
        ~Solver(){}
};

int main()
{
    cv::Mat *pt_it = new cv::Mat(800, 800, CV_8UC3);

    Plot<std::string> pt_class("init_class", pt_it);
    Solver sv(pt_class, pt_it);

    pt_class.~Plot();
    cv::imshow("plot_init", *pt_it);
    cv::waitKey(0);
    
    delete(pt_it);
    
    return 0;
}