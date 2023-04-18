#include <ros/ros.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Eigen/Dense>

#include <g2o/core/base_vertex.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/types/sba/types_six_dof_expmap.h>

struct Measurement
{
    Measurement ( Eigen::Vector3d p, float g ) : pos_world ( p ), grayscale ( g ) {}
    Eigen::Vector3d pos_world;
    float grayscale;
};

struct CAMERA_INTRINSIC_PARAM
{
    float cx;
    float cy;
    float fx;
    float fy;
    float depth_scale;
}cip={325.5, 253.5, 518.0, 519.0, 1000.0};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "talker");
	ros::NodeHandle nh;
	ros::Rate loop_rate(30);

    srand((unsigned int) time(0));
    cv::Mat color, depth, gray;
    std::vector<Measurement> measurements;
    Eigen::Matrix3f K;
    K<<cip.fx,
        0.f,
            cip.cx,
                0.f,
                    cip.fy,
                        cip.cy,
                            0.f,
                                0.f,
                                    1.0f;
    Eigen::Isometry3d Tcw = Eigen::Isometry3d::Identity();
    while(ros::ok()){
        cv::Mat prev_color;

        // ros::spinOnce();
        loop_rate.sleep();
    }


    return 0;
}