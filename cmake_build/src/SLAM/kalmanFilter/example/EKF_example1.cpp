#include <iostream>
#include "../Extended_kalmanFilter.hpp"

/*
    [2-Wheel Robot State Estimation using Extended Kalman Filter]

    This example demonstrates the use of the Extended Kalman Filter (EKF) for state estimation, 
    where the state vector consists of three elements: [x, y, yaw], 
    and the measurement vector also consists of three elements: [x, y, yaw].
    The state transition model is a simple velocity motion model, and the observation model is represented by an identity matrix. 
    The prediction step is performed using the motion model, while the correction step is carried out using the observation model.
*/

constexpr size_t DIM_X = 3;     // State vector dimension: [x, y, yaw]
constexpr size_t DIM_Z = 3;     // Measurement vector dimension

ekf::Vector<DIM_X> x_init;      // Initial state vector
ekf::Vector<DIM_X> u;           // Encoder velocity(x_vel, y_vel, yaw_vel)
ekf::float32_t dt = 0.1;        // Time step
ekf::Vector<DIM_Z> z;           // IMU measurement(x_vel, y_vel, yaw_vel)    
ekf::Matrix<DIM_Z, DIM_X> H;    // IMU measurement Jacobian matrix
ekf::Matrix<DIM_Z, DIM_Z> R;    // IMU measurement noise covariance matrix

int main(int argc, char** argv)
{
    ekf::Extended_KalmanFilter<DIM_X, DIM_Z> ekf;

    x_init << 0.0, 0.0, 0.0;
    ekf.vecX() = x_init;

    // Encoder velocity (x_vel, y_vel, yaw_vel)
    u << 1.1, 0.0, 0.2;
    // IMU measurement (x_vel, y_vel, yaw_vel)
    z << 0.0, 0.0, 0.21;

    ekf.predict(u, dt);

    R.setIdentity();
    R *= 0.01;
    H = ekf.observation_model();
    ekf.correct(z, R, H);

    std::cout << "Estimated state:\n" << ekf.vecX() << std::endl;
    std::cout << "Estimated covariance:\n" << ekf.matP() << std::endl;

    return 0;
}