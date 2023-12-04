#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include <deque>
#include <Eigen/Dense>

const static int At_least_init_point = 3;
const static int At_least_buffer_size = 3;
const static float k_max_threshold = 4.0;

class Cluster
{
public:
    int points_size_;
    std::deque<cv::Point2f> D_points_;
    cv::Mat M_points;

    cv::Mat mean_;
    cv::Mat cov_;
    cv::Point2f normal_vector_;
    double orthogonal_standard_deviation_distance_;
public:
    void init_data(cv::Point2f& data){
        D_points_.push_back(data);
        points_size_ = D_points_.size();
    }
    void convert_Deque2Mat(void){
        M_points = cv::Mat(points_size_, 2, CV_64F);
        int cnt=0;
        std::for_each(D_points_.begin(), D_points_.end(), [&cnt, this](cv::Point2f& pt){
            M_points.at<float>(cnt, 0) = pt.x;
            M_points.at<float>(cnt, 1) = pt.y;
            cnt++;
        });
    }
    void init_mean(void){cv::reduce(M_points, mean_, 0, cv::REDUCE_AVG);}
    void init_covariance(void){
        cv::Mat centered = M_points - cv::repeat(mean_, points_size_, 1);
        cov_ = (centered.t() * centered) / double(points_size_ - 1);
        std::cout << "cov = " << cov_ << std::endl;       
    }
    void init_normalVector(void){
        double theta = cov_.at<double>(0,1) / cov_.at<double>(0,0);
        normal_vector_.x = -(theta / (sqrt(pow(theta, 2)+1)));
        normal_vector_.y = 1.0 / (sqrt(pow(theta, 2)+1));
        std::cout << "normal_v [" << normal_vector_.x << ", " << normal_vector_.y << "]" << std::endl;
    }
    void calc_all_orthogonal_distance(void){
        cv::Mat Zero_Mean_points = M_points.clone();
        for(int row=0; row<points_size_; row++){
            Zero_Mean_points.at<double>(row,0) = M_points.at<double>(row,0) - mean_.at<double>(0);
            Zero_Mean_points.at<double>(row,1) = M_points.at<double>(row,1) - mean_.at<double>(1);
        }        

        cv::Point2d dummy;
        cv::Mat orthogonal_distance = cv::Mat(points_size_, 1, CV_64F);
        for(int row=0; row<points_size_; row++){
            cv::Point2d dummy = cv::Point2d(Zero_Mean_points.at<double>(row,0), Zero_Mean_points.at<double>(row,1));
            orthogonal_distance.at<double>(row) = fabs((dummy.x * normal_vector_.x) + (dummy.y * normal_vector_.y));
        }
        double dis_mean = cv::mean(orthogonal_distance)[0];
        double variance = 0;
        for (int i = 0; i < orthogonal_distance.rows; ++i) {
            double diff = orthogonal_distance.at<double>(i, 0) - dis_mean;
            variance += diff * diff;
        }
        variance /= points_size_;
        std::cout << "variance: " << variance << std::endl;
        std::cout << "Standard_deviation: " << sqrt(variance) << std::endl;
        orthogonal_standard_deviation_distance_ = sqrt(variance);
    }
    float calc_orthogonal_distance(cv::Point2d& query){
        cv::Point2d dummy = cv::Point2d(query.x - mean_.at<double>(0), query.y - mean_.at<double>(1));
        float normal_dis = fabs((dummy.x * normal_vector_.x) + (dummy.y * normal_vector_.y));
        std::cout << "Normal_dis = " << normal_dis << std::endl;
        return normal_dis;                
    }
    bool operator()()
    {   
        if(points_size_ == 0){return false;}
        convert_Deque2Mat();
        init_mean();
        init_covariance();
        init_normalVector();
        calc_all_orthogonal_distance();   
        return true;
    }
    Cluster():points_size_(0), orthogonal_standard_deviation_distance_(-1){};
    ~Cluster(){};  
};

class Buffer
{
public:
    Cluster candidate_points_;
    void init_data(cv::Point2f& data){
        candidate_points_.init_data(data);

        if(candidate_points_.points_size_ >= At_least_buffer_size){consistent();}
    }
    void consistent(){
        candidate_points_();
        
    }
    Buffer(){};
    ~Buffer(){};
};

class Plot
{
public:
    cv::Mat output;
    void operator()(cv::Mat& data, std::vector<Cluster>& clusters){
        if(data.empty() == true){return;};

        output = cv::Mat(500,500,CV_8UC3, cv::Scalar(0,0,0));
        for(int iter=0; iter<data.rows; iter++){
            cv::circle(output, cv::Point(data.at<double>(iter,0), data.at<double>(iter,1)), 1, cv::Scalar(255,255,255), -1);
        }
        if(clusters.size() > 0){
            std::for_each(clusters.begin(), clusters.end(), [this](Cluster& cluster){
                std::for_each(cluster.D_points_.begin(), cluster.D_points_.end(), [this](cv::Point2d pt){
                    cv::circle(output, pt, 1, cv::Scalar(0,0,255), -1);    
                });
            });
        }
        cv::imshow("Canvas", output);
        cv::waitKey(0);
    }
};

class Point_Generator
{
private:
    void create_pts(void){
        std::vector<Cluster> clusters;
        Plot plot;
        points_ = cv::Mat(20, 2, CV_64F);
        for(int iter=0; iter<20; iter++){
            points_.at<double>(iter, 0) = static_cast<double>(iter+100);
            points_.at<double>(iter, 1) = static_cast<double>(300);
            std::cout << "Generate Point (x,y) = (" << static_cast<double>(iter+100) << ", " << static_cast<double>(300) << ")" << std::endl;
            if(clusters.size() == 0){
                //first cluster!!!!
                
            }

            plot(points_, clusters);
        }
    }
public: 
    cv::Mat points_;
    Point_Generator(){
        create_pts();
    };
    ~Point_Generator() = default;
};

int main(int argc, char** argv)
{
    Point_Generator pg;

    return 0;
}