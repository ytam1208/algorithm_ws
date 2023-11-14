#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class Point_Generator
{
private:

    void create_pts(void){
        points.reserve(10);
        for(int iter=0; iter<100; iter++){
            points.emplace_back(std::move(cv::Point(iter*5, iter*5)));
        }
        
        points_ = cv::Mat(100, 2, CV_64F);
        for(int iter=0; iter<100; iter++){
            points_.at<double>(iter, 0) = static_cast<double>(iter*5);
            points_.at<double>(iter, 1) = static_cast<double>(iter*5);
        }
    }
public: 
    cv::Mat points_;
    std::vector<cv::Point> points;
    Point_Generator(){
        create_pts();
    };
    ~Point_Generator() = default;
};

class Dummy_PCA
{
public:
    std::array<double, 2> mean;
    std::array<std::array<double, 2>, 2> cov;

    cv::Point eigen_mean;
    std::vector<cv::Point2d> eigen_vecs;
    std::vector<double> eigen_val;
public:
    void Get_mean(std::vector<cv::Point> pts){
        std::for_each(pts.begin(), pts.end(), [&,this](cv::Point& pt){
            mean[0]+=static_cast<double>(pt.x);
            mean[1]+=static_cast<double>(pt.y);
        });
        mean[0] /= static_cast<double>(pts.size());
        mean[1] /= static_cast<double>(pts.size());
        std::cout << "Mean = " << mean[0] << ", " << mean[1] << std::endl;
    }
    void Get_covariacne(std::vector<cv::Point> pts){
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                std::for_each(pts.begin(), pts.end(), [&,this](cv::Point& pt){
                    cov[i][j] += (static_cast<double>(pt.x) - mean[0]) * (static_cast<double>(pt.y) - mean[1]) / static_cast<double>(pts.size()-1);
                });                
            }
        }
        std::cout << "Cov[0][0] = " << cov[0][0] << " \nCov[0][1] = " << cov[0][1] << std::endl;
        std::cout << "Cov[1][0] = " << cov[1][0] << " \nCov[1][1] = " << cov[1][1] << std::endl;
    }
    void CV_PCA(cv::Mat pts){
        cv::PCA pca_analysis(pts, cv::Mat(), cv::PCA::DATA_AS_ROW);
        cv::Point cntr = cv::Point(static_cast<int>(pca_analysis.mean.at<double>(0, 0)),
                                    static_cast<int>(pca_analysis.mean.at<double>(0, 1)));

        eigen_vecs.reserve(2);
        eigen_val.reserve(2);
        for(int i=0; i<2; i++){
            eigen_vecs[i] = cv::Point2d(pca_analysis.eigenvectors.at<double>(i,0),
                                        pca_analysis.eigenvectors.at<double>(i,1));
            eigen_val[i] = pca_analysis.eigenvalues.at<double>(i);
        }
        eigen_mean = cntr;
        std::cout << "PCA Mean = " << cntr.x << ", " << cntr.y << std::endl;        
    }
    Dummy_PCA()=default;
    ~Dummy_PCA()=default;
};

class Plot
{
public:
    cv::Mat output;
    void projection_eign(std::vector<cv::Point2d>& eigen_vec, std::vector<double>& eigen_val, cv::Point& eigen_mean){
        cv::Point p1 = eigen_mean+0.02 * cv::Point(static_cast<int>(eigen_vec[0].x * eigen_val[0]), static_cast<int>(eigen_vec[0].y * eigen_val[0]));
        cv::Point p2 = eigen_mean+0.02 * cv::Point(static_cast<int>(eigen_vec[1].x * eigen_val[1]), static_cast<int>(eigen_vec[1].y * eigen_val[1]));
        


    }
    void projection(std::vector<cv::Point> pts){
        if(pts.size() == 0){return;};
        for(int iter=0; iter<pts.size(); iter++){
            cv::Point pt = pts[iter];
            output.at<cv::Vec3b>(pt.x, pt.y)[0]=255;
            output.at<cv::Vec3b>(pt.x, pt.y)[1]=255;
            output.at<cv::Vec3b>(pt.x, pt.y)[2]=255;
        }
    }
    void imshow(void){
        bool is_empty = true;
        if(output.empty() == is_empty){}
        else{
            cv::imshow("output", output);
            cv::waitKey(10000);
        }
    }
public:
  Plot(Point_Generator &pts){
    output = cv::Mat(500,500,CV_8UC3, cv::Scalar(0,0,0));
    projection(pts.points);

    imshow();
  };
  ~Plot()=default;  
};

int main(int argc, char** argv)
{
    Point_Generator pg;
    Plot pt(pg);

    Dummy_PCA pca;
    pca.Get_mean(pg.points);
    pca.Get_covariacne(pg.points);
    pca.CV_PCA(pg.points_);
    pt.projection_eign(pca.eigen_vecs, pca.eigen_val, pca.eigen_mean);
    
    pt.imshow();
    return 0;
}