#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class Point_Generator
{
private:

    void create_pts(void){
        points_ = cv::Mat(200, 2, CV_64F);
        for(int iter=0; iter<100; iter++){
            points_.at<double>(iter, 0) = static_cast<double>(iter+100);
            points_.at<double>(iter, 1) = static_cast<double>(300);
        }
    }
public: 
    cv::Mat points_;
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
    cv::Point2d normal_vector;
public:
    void Get_mean(cv::Mat pts){
        for(int i=0; i<pts.rows; i++){
            mean[0] += pts.at<double>(i,0);
            mean[1] += pts.at<double>(i,1);
        }
        mean[0] /= pts.rows;
        mean[1] /= pts.rows;
        std::cout << "Mean = " << mean[0] << ", " << mean[1] << std::endl;
    }
    void Get_covariacne(cv::Mat pts){
        for(int i=0; i<2; i++){
            for(int j=0; j<2; j++){
                for(int row=0; row<pts.rows; row++)
                    cov[i][j] += (pts.at<double>(i,0) - mean[0]) * (pts.at<double>(i,1) - mean[1]) / static_cast<double>(pts.rows);        
            }
        }
        std::cout << "x_Cov[0][0] = " << cov[0][0] << " \nx,y_Cov[0][1] = " << cov[0][1] << std::endl;
        std::cout << "x,y_Cov[1][0] = " << cov[1][0] << " \ny_Cov[1][1] = " << cov[1][1] << std::endl;
    }
    void Get_normalVector(void){
        double theta = cov[0][1] / cov[0][0];
        normal_vector.x = -theta / (sqrt(pow(theta, 2)+1));
        normal_vector.y = 1.0 / (sqrt(pow(theta, 2)+1));
        std::cout << "normal_v [" << normal_vector.x << ", " << normal_vector.y << "]" << std::endl;
    }
    double normalVector_calc(cv::Point2d query){
        cv::Point2d dummy = cv::Point2d(query.x - mean[0], query.y - mean[1]);
        std::cout << "dummy x[" << dummy.x << "] y[" << dummy.y <<"] normal_v[" << normal_vector.x << ", " << normal_vector.y <<"]" << std::endl;
        double normal_dis = fabs((dummy.x * normal_vector.x) + (dummy.y * normal_vector.y));
        std::cout << "Normal_dis = " << normal_dis << std::endl;
        return normal_dis;
    }
    double calc_orthogonal_distance(cv::Mat pts){
        cv::Point2d dummy;
        
        cv::Mat orthogonal_distance = cv::Mat(pts.rows, 1, CV_64F);
        for(int row=0; row<pts.rows; row++){
            cv::Point2d dummy = cv::Point2d(pts.at<double>(row,0) - mean[0], pts.at<double>(row,1) - mean[1]);
            orthogonal_distance.at<double>(row,0) = fabs((dummy.x * normal_vector.x) + (dummy.y * normal_vector.y));
        }
        
        double orthogonal_mean_distance;
        for(int row=0; row<orthogonal_distance.rows; row++){
            orthogonal_mean_distance += orthogonal_distance.at<double>(row);
        }
        orthogonal_mean_distance /= orthogonal_distance.rows;

        cv::Mat orthogonal_deviation_distance = orthogonal_distance.clone();
        for(int row=0; row<orthogonal_deviation_distance.rows; row++){
            orthogonal_deviation_distance.at<double>(row) -= orthogonal_mean_distance;
        }
        cv::Mat Pow_orthogonal_deviation_distance = orthogonal_deviation_distance.clone();
        for(int row=0; row<orthogonal_deviation_distance.rows; row++){
            Pow_orthogonal_deviation_distance.at<double>(row) = pow(orthogonal_deviation_distance.at<double>(row),2);
        }        
        double orthogonal_variance_distance;
        for(int row=0; row<Pow_orthogonal_deviation_distance.rows; row++){
            orthogonal_variance_distance += Pow_orthogonal_deviation_distance.at<double>(row);
        }
        orthogonal_variance_distance /= orthogonal_distance.rows;

        std::cout << "Orthogonal_variance_distance = " << orthogonal_variance_distance << std::endl;
        double orthogonal_standard_deviation_distance = sqrt(orthogonal_variance_distance);
        std::cout << "Standard_deviation_Orthogonal_variance_distance = " << orthogonal_standard_deviation_distance << std::endl;
        
        return orthogonal_standard_deviation_distance;
    }
    void clustering_conditional_1(double query_distance, double cluster_distance_variance){
        double k_max = 4.0;
        double threshold_orthogonal_distance = k_max * cluster_distance_variance;
        std::cout << "Clustering orthogonal_distance threshold[" << threshold_orthogonal_distance << "]" << std::endl;
        if(query_distance < threshold_orthogonal_distance){std::cout << "Neighbor point!! Query_dis[" << query_distance << "]" << std::endl;}
        else{std::cout << "No Neighbor point Query_dis[" << query_distance << "]" << std::endl;}
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
            std::cout << "Eigen vecs[" <<i << "](" << eigen_vecs[i] <<")" << std::endl;
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
    void draw_axis(cv::Point mean, cv::Point eigen_pt, cv::Scalar color, const float scale = 0.2){
        double angle = atan2( (double) mean.y - eigen_pt.y, (double) mean.x - eigen_pt.x ); // angle in radians
        double hypotenuse = sqrt( (double) (mean.y - eigen_pt.y) * (mean.y - eigen_pt.y) + (mean.x - eigen_pt.x) * (mean.x - eigen_pt.x));

        eigen_pt.x = (int)(mean.x - scale * hypotenuse * cos(angle));
        eigen_pt.y = (int)(mean.y - scale * hypotenuse * sin(angle));
        cv::line(output, mean, eigen_pt, color, 1, cv::LINE_AA);

        mean.x = (int)(eigen_pt.x + 9 * cos(angle + CV_PI / 4));
        mean.y = (int)(eigen_pt.y + 9 * sin(angle + CV_PI / 4));
        cv::line(output, mean, eigen_pt, color, 1, cv::LINE_AA);

        mean.x = (int)(eigen_pt.x + 9 * cos(angle - CV_PI / 4));
        mean.y = (int)(eigen_pt.y + 9 * sin(angle - CV_PI / 4));
        cv::line(output, mean, eigen_pt, color, 1, cv::LINE_AA);
    }
    void projection_eign(std::vector<cv::Point2d>& eigen_vec, std::vector<double>& eigen_val, cv::Point& eigen_mean){
        cv::circle(output, eigen_mean, 3, cv::Scalar(255,0,255), 2);
        
        cv::Point p1 = eigen_mean+0.02 * cv::Point(static_cast<int>(eigen_vec[0].x * eigen_val[0]), static_cast<int>(eigen_vec[0].y * eigen_val[0]));
        cv::Point p2 = eigen_mean+0.02 * cv::Point(static_cast<int>(eigen_vec[1].x * eigen_val[1]), static_cast<int>(eigen_vec[1].y * eigen_val[1]));
        
        draw_axis(eigen_mean, p1, cv::Scalar(0,255,0), 1);
        draw_axis(eigen_mean, p2, cv::Scalar(255,255,0), 1);
    }
    void projection(cv::Mat pts){
        if(pts.empty() == true){return;};
        for(int iter=0; iter<pts.rows; iter++){
            cv::circle(output, cv::Point(pts.at<double>(iter,0), pts.at<double>(iter,1)), 1, cv::Scalar(255,255,255), -1);
        }
    }
    void imshow(void){
        bool is_empty = true;
        if(output.empty() == is_empty){}
        else{
            cv::imshow("output", output);
            cv::waitKey(0);
        }
    }
public:
  Plot(Point_Generator &pts){
    output = cv::Mat(500,500,CV_8UC3, cv::Scalar(0,0,0));
    projection(pts.points_);
    imshow(0);
  };
  ~Plot()=default;  
  void operator()(cv::Point2d query_data){
    cv::circle(output, query_data, 3, cv::Scalar(0,0,255), 2);
  }
};

int main(int argc, char** argv)
{
    cv::Point2d Query_data = cv::Point(208,300);    //199,300,,,,limit 208,300 

    Point_Generator pg;
    Plot pt(pg);

    Dummy_PCA pca;
    pca.Get_mean(pg.points_);
    pca.Get_covariacne(pg.points_);   
    pca.Get_normalVector();

    pca.CV_PCA(pg.points_);
    pt.projection_eign(pca.eigen_vecs, pca.eigen_val, pca.eigen_mean);
    pt(Query_data);
    double normal_vector_dis = pca.normalVector_calc(Query_data);
    double orthogonal_standard_deviation_distance = pca.calc_orthogonal_distance(pg.points_);
    pca.clustering_conditional_1(normal_vector_dis, orthogonal_standard_deviation_distance);

    pt.imshow();
    return 0;
}