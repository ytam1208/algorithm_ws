#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "stdlib.h"
#include <unistd.h>
#include <ceres/ceres.h>

struct CIRCLE_FITTING_COST
{
    CIRCLE_FITTING_COST ( float x, float y ) : _x ( x ), _y ( y ) {}
    template <typename T>
    bool operator()(const T* const abr, T* residual)  //모델 파라미터, 3차원 배열
    const    // redisual 
    {
        residual[0] = sqrt(pow(T(_x)-abr[0], 2) + pow(T(_y)-abr[1], 2)) - abr[2];
        return true;
    }
    const float _x, _y; 
};

template <typename T1>
class Plot
{
    public:
        const T1 this_name;
        int circle_size;
        std::vector<cv::Point2f> Point_v;

        void init_val(cv::Mat* init, cv::Mat* convert){
            srand(time(NULL));
            circle_size = 300;
            std::cout << "Origin a,b,r = " << circle_size << ", " << init->rows/2 << ", " << init->cols/2 << std::endl;
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
                        Point_v.push_back(cv::Point(i,j));
                }
        }

        Plot(){}
        Plot(std::string name):this_name(name){}
        Plot(std::string name, cv::Mat* init, cv::Mat* convert):this_name(name){
            init_val(init, convert);
        }
        ~Plot(){
            // std::cout << "Destroy [" << this_name << "] " << std::endl;
        }
};

template <typename Param1, typename Param2, typename Param3>
class Solver : public Plot<std::string>
{
    private:
        Plot<std::string> pt;
        int optimize_iter = 0;
        double error = 0.001;  

    public:     
        double function(cv::Point Point, Param1 input_a, Param2 input_b, Param3 input_r){  //(x-a)^2 + (y-b)^2 = r^2
            return sqrt(pow(Point.x-input_a, 2) + pow(Point.y-input_b, 2));
        }
        cv::Mat update(cv::Mat& J, cv::Mat& F){
            return J.inv(cv::DECOMP_SVD) * F;   // ((J.t() * J).inv()) * J.t() * F;
        }
        bool Check_minimize(cv::Mat* Origin_x, cv::Mat* New_x){
            float New_a = New_x->at<float>(0,0);    
            float New_b = New_x->at<float>(1,0);
            float New_r = New_x->at<float>(2,0);

            if(abs(New_a)<= error)
                if(abs(New_b) <= error)
                    if(abs(New_r) <= error)
                        return true;
            for(int i = 0; i < 3; i++)
                Origin_x->at<float>(i,0) = Origin_x->at<float>(i,0) - New_x->at<float>(i,0);
            return false;
        }
        void Make_Jacobian(int i, cv::Mat* J, cv::Mat* init_X_mat, cv::Point2f ob_point){
            J->at<float>(i,0) = (init_X_mat->at<float>(0,0) - ob_point.x)/function(ob_point, init_X_mat->at<float>(0,0), init_X_mat->at<float>(1,0), init_X_mat->at<float>(2,0));
            J->at<float>(i,1) = (init_X_mat->at<float>(1,0) - ob_point.y)/function(ob_point, init_X_mat->at<float>(0,0), init_X_mat->at<float>(1,0), init_X_mat->at<float>(2,0));
            J->at<float>(i,2) = -1.0f;
        }
        void Gauss_Newton(std::vector<cv::Point2f>& Point_v, Param1 input_a, Param2 input_b, Param3 input_r, cv::Mat* visual){
            cv::Mat input_x = cv::Mat::zeros(3, 1, CV_32FC1);
            input_x.at<float>(0,0) = (float)input_a;
            input_x.at<float>(1,0) = (float)input_b;
            input_x.at<float>(2,0) = (float)input_r;

            cv::Mat New_x = cv::Mat::zeros(3, 1, CV_32FC1);  //a,b,r Parameter
            cv::Mat Jacobian = cv::Mat::zeros(Point_v.size(), 3, CV_32FC1);   //F`
            cv::Mat F = cv::Mat::zeros(Point_v.size(), 1, CV_32FC1);  //Parameter function Metrix
            Use_Ceres(visual, &input_x, Point_v);
            while(1){
                cv::Mat Show_mat = cv::Mat::zeros(800, 800, CV_8UC3);
                Show_mat += *visual;
                optimize_iter++;
                for(int i = 0; i < Point_v.size(); i++){
                    Make_Jacobian(i, &Jacobian, &input_x, Point_v[i]);
                    F.at<float>(i,0) = function(Point_v[i], input_x.at<float>(0,0), input_x.at<float>(1,0), input_x.at<float>(2,0)) - input_x.at<float>(2,0);
                }       
                New_x = update(Jacobian, F);
                cv::circle(Show_mat, cv::Point(input_x.at<float>(0,0), input_x.at<float>(1,0)), fabs((int)input_x.at<float>(2,0)), cv::Scalar(0,255,0), 1, 8);
                cv::imshow("Convert", Show_mat);

                if(Check_minimize(&input_x, &New_x)){
                    std::cout << "["<< optimize_iter << "]Iteration Optimize!!! a,b,r = " << input_x.at<float>(0,0) << ", " << input_x.at<float>(1,0) << ", " << input_x.at<float>(2,0) << std::endl;
                    cv::waitKey(0);
                    break;
                }
                std::cout << "Now Optimize a,b,r = " << input_x.at<float>(0,0) << ", " << input_x.at<float>(1,0) << ", " << input_x.at<float>(2,0) << std::endl;
                cv::waitKey(0);
            }

        }
        void Use_Ceres(cv::Mat* visual, cv::Mat* Paramter, std::vector<cv::Point2f>& Point_v)
        {
            cv::Mat Show_mat = cv::Mat::zeros(800, 800, CV_8UC3); 
            Show_mat += *visual;

            int N = 100;
            double abr[3];
            abr[0] = Paramter->at<float>(0,0);
            abr[1] = Paramter->at<float>(1,0);
            abr[2] = Paramter->at<float>(2,0);
            std::cout << "Use_Ceres = " << abr[0] << "," << abr[1] << "," << abr[2] << std::endl;

            ceres::Problem problem;
            for(int i = 0; i < Point_v.size(); i++){
                problem.AddResidualBlock(
                    new ceres::AutoDiffCostFunction<CIRCLE_FITTING_COST, 1, 3>(
                        new CIRCLE_FITTING_COST(Point_v[i].x, Point_v[i].y)
                    ),
                    nullptr,
                    abr
                );
            }
            ceres::Solver::Options options; 
            options.minimizer_progress_to_stdout = true;
            ceres::Solver::Summary summary;                
            // chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
            ceres::Solve ( options, &problem, &summary );  
            // chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
            // chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>( t2-t1 );
            // std::cout<<"solve time cost = "<<time_used.count()<<" seconds. "<< std::endl;

            std::cout<<summary.BriefReport() << std::endl;
            std::cout<<"estimated a,b,c = ";
            for ( auto a:abr ) std::cout<< a <<" ";
            std::cout<<std::endl;

            cv::circle(Show_mat, cv::Point(abr[0], abr[1]), fabs(abr[2]), cv::Scalar(0,255,0), 1, 8);
            cv::imshow("Convert", Show_mat);
            cv::waitKey(0);
        }
        Solver(){}
        Solver(std::vector<cv::Point2f>& Points, Param1 input_a, Param2 input_b, Param3 input_r, cv::Mat* visual){
            Gauss_Newton(Points, input_a, input_b, input_r, visual);
        }
        ~Solver(){}
};

int main()
{
    cv::Mat* init_mat = new cv::Mat(800, 800, CV_8UC3);
    cv::Mat* convert_mat = new cv::Mat(800, 800, CV_8UC3);

    Plot<std::string> pt("init", init_mat, convert_mat);
    Solver<double, double, double> sv(pt.Point_v, 500, 400, 10, convert_mat);

    // delete(init_mat);
    // pt.~Plot();
    return 0;
}