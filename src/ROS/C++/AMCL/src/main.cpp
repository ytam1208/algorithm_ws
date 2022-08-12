#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h>
#include "std_msgs/String.h"
#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>

std::vector<double> cov_x_v;
std::vector<double> cov_y_v;
std::vector<double> cov_th_v;

void poseCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr &pose_msg)
{
    double cov_x, cov_y, cov_z, norm_cov;
    cov_x = pose_msg->pose.covariance[0];
    cov_y = pose_msg->pose.covariance[7];
    cov_z = pose_msg->pose.covariance[35];

    ROS_INFO("cov_x: %f", cov_x);
    ROS_INFO("cov_y: %f", cov_y);
    ROS_INFO("cov_z: %f", cov_z);

    cov_x_v.push_back(cov_x);
    cov_y_v.push_back(cov_y);
    cov_th_v.push_back(cov_z);

    std::string path = "/home/cona/";

    std::string folder_path = path + "amcl_data";
    mkdir(folder_path.c_str(), 0777);

    std::ofstream writeFile;
    writeFile.open(folder_path + "/AMCL_x.txt");
    for (int i = 0; i < cov_x_v.size(); i++)
        writeFile << cov_x_v[i] << "\n";
    writeFile.close();

    writeFile.open(folder_path + "/AMCL_y.txt");
    for (int i = 0; i < cov_y_v.size(); i++)
        writeFile << cov_y_v[i] << "\n";
    writeFile.close();

    writeFile.open(folder_path + "/AMCL_th.txt");
    for (int i = 0; i < cov_th_v.size(); i++)
        writeFile << cov_th_v[i] << "\n";
    writeFile.close();

    ROS_INFO("Write txt!");
}

// void msgCallback(const std_msgs::String::ConstPtr& msg)
// {
//     if(msg->data == "done"){
//     }
// }

int main(int argc, char **argv)
{
    ros::init(argc, argv, "AMCL_read");
    ros::NodeHandle nh;

    ros::Subscriber pose_cov = nh.subscribe("amcl_pose", 1, &poseCallback);
    // ros::Subscriber std_ = nh.subscribe("command", 1, &msgCallback);

    ros::spin();
    return 0;
}