#include <iostream>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class Plot
{
public:
    double Meter2Pixel = 1.0 * 20.0;
    cv::Mat output;
    Plot(){output = cv::Mat(500,500,CV_8UC3, cv::Scalar(0,0,0));}
    Plot(const std::vector<Eigen::Vector3d>& data){
        output = cv::Mat(500,500,CV_8UC3, cv::Scalar(0,0,0));
        for(int iter=0; iter<data.size(); iter++){
            double x = data[iter].x() * Meter2Pixel + 250.0;
            double y = data[iter].y() * Meter2Pixel + 250.0;
            cv::circle(output, cv::Point(x, y), 1, cv::Scalar(255,255,255), -1);
        }
        view();
    }
    void draw(const Eigen::Vector3d& point, cv::Scalar&& color)
    {
        double x = point.x() * Meter2Pixel + 250.0;
        double y = point.y() * Meter2Pixel + 250.0;
        cv::circle(output, cv::Point(x, y), 1, color, -1);
        view();
    }
    void view()
    {
        cv::imshow("Canvas", output);
        cv::waitKey(0);
    }
};

std::vector<Eigen::Vector3d> NearestPoint;

int main(int argc, char** argv)
{
    NearestPoint.push_back(Eigen::Vector3d(1.1, 1.1, 1.0));
    NearestPoint.push_back(Eigen::Vector3d(2.2, 1.0, 1.0));
    NearestPoint.push_back(Eigen::Vector3d(3.3, 1.0, 1.0));
    NearestPoint.push_back(Eigen::Vector3d(4.4, 1.1, 1.0));
    NearestPoint.push_back(Eigen::Vector3d(5.7, 1.0, 1.0));
    for(auto iter : NearestPoint)
    {
        std::cout << "input xyz = " << iter.x() << ", " << iter.y() << ", " << iter.z() << std::endl;
    }
    Plot pt(NearestPoint);

    Eigen::Vector3d centerPt = Eigen::Vector3d::Zero();
    for(auto iter : NearestPoint)
    {
        centerPt += iter;
    }
    centerPt /= NearestPoint.size();
    pt.draw(centerPt, cv::Scalar(0,0,255));
    std::cout << "center = " << centerPt.x() << ", " << centerPt.y() << ", " << centerPt.z() << std::endl;

    Eigen::Matrix3d variance = Eigen::Matrix3d::Zero();
    for(auto iter : NearestPoint)
    {
        Eigen::Vector3d zero_center = iter - centerPt;
        variance(0,0) = zero_center.x() * zero_center.x(); 
        variance(0,1) = zero_center.y() * zero_center.x();
        variance(0,2) = zero_center.z() * zero_center.x();

        variance(1,0) = zero_center.x() * zero_center.y();
        variance(1,1) = zero_center.y() * zero_center.y();
        variance(1,2) = zero_center.z() * zero_center.y();

        variance(2,0) = zero_center.x() * zero_center.z();
        variance(2,1) = zero_center.y() * zero_center.z();
        variance(2,2) = zero_center.z() * zero_center.z();
    }
    std::cout << "Variance = \n" << variance.matrix() << std::endl;

    Eigen::EigenSolver<Eigen::Matrix<double, 3, 3>> eigenSolver(variance);
    Eigen::Matrix<double, 3, 1> Eigen_value = eigenSolver.eigenvalues().real();
    Eigen::Matrix<double, 3, 3> Eigen_vector = eigenSolver.eigenvectors().real();    //eigenvectors Matrix
    
    std::cout << "Eigen_value = " << Eigen_value.x() << ", " << Eigen_value.y() << ", " << Eigen_value.z() << std::endl;
    std::cout << "Eigen_vector = \n" << Eigen_vector.matrix() << std::endl;

    Eigen::Vector3d center_to_Right_pt;
    center_to_Right_pt.x() = centerPt.x() + 0.1 * Eigen_vector(0,0);
    center_to_Right_pt.y() = centerPt.y() + 0.1 * Eigen_vector(0,1);
    center_to_Right_pt.z() = centerPt.z() + 0.1 * Eigen_vector(0,2);
    std::cout << "center_to_Right_pt = " << center_to_Right_pt.x() << ", " << center_to_Right_pt.y() << ", " << center_to_Right_pt.z() << std::endl;
    pt.draw(center_to_Right_pt, cv::Scalar(255,0,0));

    Eigen::Vector3d center_to_left_pt;
    center_to_left_pt.x() = centerPt.x() - 0.1 * Eigen_vector(0,0);
    center_to_left_pt.y() = centerPt.y() - 0.1 * Eigen_vector(0,1);
    center_to_left_pt.z() = centerPt.z() - 0.1 * Eigen_vector(0,2);
    std::cout << "center_to_left_pt = " << center_to_left_pt.x() << ", " << center_to_left_pt.y() << ", " << center_to_left_pt.z() << std::endl;
    pt.draw(center_to_left_pt, cv::Scalar(0,255,0));

    return 0;
}