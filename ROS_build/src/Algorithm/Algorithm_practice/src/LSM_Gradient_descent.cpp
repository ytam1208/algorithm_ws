#include <opencv2/opencv.hpp>

template <typename T1>
class Curve
{
    public:
        const T1 this_name;
        float* Curve_param;
        std::vector<cv::Point2f> curvePoints;

        void init_val(cv::Mat* init){
            float start_point_x = 20;
            float end_point_x = 120;
            for(float x = start_point_x; x <= end_point_x; x+=1){
                float y = Curve_param[0]*x*x - Curve_param[1]*x + Curve_param[2];
                cv::Point2f new_point = cv::Point2f(x*4, y*3);                 
                curvePoints.push_back(new_point);                      
            }    
            cv::Mat curve(curvePoints, true);
            curve.convertTo(curve, CV_32S); //adapt type for polylines
            cv::polylines(*init, curve, false, cv::Scalar(255,255,255), 2, cv::LINE_AA);
            cv::circle(*init, Select_first_pt(), 10, cv::Scalar(0,0,255),-1,8,0);
            cv::imshow("Curve-polylines", *init);
            // cv::waitKey();
        }
        inline cv::Point2f Select_first_pt(){
            return curvePoints[0];
        }
        Curve(std::string name):this_name(name){}
        Curve(std::string name, cv::Mat* input, float* param):
        this_name(name), Curve_param(param){
            init_val(input);
        }
};

class Solver
{
    public:
        float* Param;
        std::vector<cv::Point2f> pt_v;
        cv::Mat Now_pt = cv::Mat::zeros(2, 1, CV_32FC1);

        float function(cv::Point2f input){
            return (Param[0] * std::powf(input.x, 2)) + Param[1]*input.x + Param[2] - input.y;
        }
        cv::Mat update(cv::Mat& Jacobian, cv::Mat& F){
            constexpr double learning_rate = 1e-2;
            cv::Mat New_pt = cv::Mat::zeros(2, 1, CV_32FC1);
            
            New_pt = Now_pt + (2.0f * learning_rate * Jacobian.t() * F);
            return New_pt;
        }
        void Make_Jacobian(int iter, cv::Mat* Jacobian, cv::Point2f& point){
            Jacobian->at<float>(iter, 0) = (2.0f * Param[0] * point.x + Param[1]) / function(point);
            Jacobian->at<float>(iter, 1) = -1.0f / function(point);
        }
        void Gradient_desenct(cv::Mat* draw_mat){
            cv::Mat F = cv::Mat::zeros(pt_v.size(), 1, CV_32FC1);
            cv::Mat Jacobian = cv::Mat::zeros(pt_v.size(), 2, CV_32FC1);
            Now_pt.at<float>(0,0) = pt_v[0].x;
            Now_pt.at<float>(1,0) = pt_v[0].y;
            
            while(1){
                for(int i = 0; i < pt_v.size(); i++){
                    Make_Jacobian(i, &Jacobian, pt_v[i]);
                    F.at<float>(i,0) = function(pt_v[i]);
                }
                cv::Mat New = update(Jacobian, F);
                std::cout << New.at<float>(0,0) << ", " << New.at<float>(1,0) << std::endl;
                cv::circle(*draw_mat, cv::Point(New.at<float>(0,0), New.at<float>(1,0)), 3, cv::Scalar(0,255,0), -1, 8, 1);
                cv::imshow("Gradient", *draw_mat);
                Now_pt = New.clone();
                cv::waitKey(1000);
            }
        }
        Solver(cv::Mat* draw_img, float* _Param, std::vector<cv::Point2f>& _input_v)
        :Param(_Param), pt_v(_input_v){
            Gradient_desenct(draw_img);
        }
};

int main(int argc, const char * argv[])
{
    cv::Mat* img = new cv::Mat(500,500, CV_8UC3, cv::Scalar(0));
    float Curve_param[]={0.0425, 6.25, 258.0}; //y = ax^2 + bx + c

    Curve<std::string> init_cv("curve", img, Curve_param);
    Solver(img, Curve_param, init_cv.curvePoints);

    return 0;
}