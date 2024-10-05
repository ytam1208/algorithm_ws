#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>

class SensorFusion {
public:
    SensorFusion() {
        imu_sub_ = nh_.subscribe("imu/data", 1000, &SensorFusion::imuCallback, this);
        odom_sub_ = nh_.subscribe("odom", 1000, &SensorFusion::odomCallback, this);
    }

private:
    void imuCallback(const sensor_msgs::Imu::ConstPtr& msg) {
        ROS_INFO("IMU data received");
        // Process IMU data
    }

    void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {
        ROS_INFO("Odometry data received");
        // Process Odometry data
    }

    ros::NodeHandle nh_;
    ros::Subscriber imu_sub_;
    ros::Subscriber odom_sub_;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "sensor_fusion_node");
    SensorFusion sensorFusion;
    ros::spin();
    return 0;
}