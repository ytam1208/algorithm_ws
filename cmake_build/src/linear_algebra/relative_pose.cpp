#include <iostream>
#include <Eigen/Dense>

template <typename T1>
void Get_XYZ(const std::string& string, const T1& data)
{
    std::cout << string << " Transformation[" << data[0] << ", " << data[1] << ", " << data[2] << "]" << std::endl;
}

template <typename T1>
void Get_RPY(const std::string& string, const T1& matrix)
{
    Eigen::Quaterniond q(matrix);
    double roll = atan2(2 * (q.w() * q.x() + q.y() * q.z()), 1 - 2 * (q.x() * q.x() + q.y() * q.y()));
    double pitch = asin(2 * (q.w() * q.y() - q.z() * q.x()));
    double yaw = atan2(2 * (q.w() * q.z() + q.x() * q.y()), 1 - 2 * (q.y() * q.y() + q.z() * q.z()));
    std::cout << string << " Rotataion[" << roll << ", " << pitch << ", " << yaw << "]" << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "------------------------------------" << std::endl;

    Eigen::Affine3d i = Eigen::Affine3d::Identity();
    i.translation() = Eigen::Vector3d(1.0, 0.0, 0.0);
    Get_XYZ("i_", i.translation());
    Get_RPY("i_", i.rotation());

    // std::cout << "i T matrix = \n" << i.matrix() << std::endl;   
    // std::cout << "T matrix = \n" << i.translation() << std::endl;
    // std::cout << "R matrix = \n" << i.rotation() << std::endl;
    std::cout << "------------------------------------" << std::endl;

    Eigen::Affine3d j = Eigen::Affine3d::Identity();
    j.translation() = Eigen::Vector3d(1.0, 1.0, 0.0);

    Eigen::AngleAxisd rollAngle(0, Eigen::Vector3d::UnitX());
    Eigen::AngleAxisd pitchAngle(0, Eigen::Vector3d::UnitY());
    Eigen::AngleAxisd yawAngle(1.5708, Eigen::Vector3d::UnitZ());

    Eigen::Matrix3d rot = (rollAngle * yawAngle * pitchAngle).toRotationMatrix();
    j.linear() = rot;
    Get_XYZ("j_", j.translation());
    Get_RPY("j_", j.rotation());
    std::cout << "------------------------------------" << std::endl;

    Eigen::Affine3d inv = i.inverse();
    Eigen::Affine3d i_to_j_delta = inv * j;
    Get_XYZ("i to j Relative ", i_to_j_delta.translation());
    Get_RPY("i to j Relative", i_to_j_delta.rotation());
    
    std::cout << "------------------------------------" << std::endl;

    Eigen::Affine3d j_to_i_delta = i_to_j_delta.inverse();
    Get_XYZ("j to i Relative ", j_to_i_delta.translation());
    Get_RPY("j to i Relative", j_to_i_delta.rotation());

    return 0;   
}