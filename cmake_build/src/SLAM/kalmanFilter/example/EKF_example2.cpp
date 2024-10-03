#include <iostream>
#include "../Extended_kalmanFilter.hpp"

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

    // Initial state vector(x, y, yaw)
    x_init << 0.0, 0.0, 0.0;
    ekf.vecX() = x_init;

    // Wheel Encoder data(x_vel, y_vel, yaw_vel)
    u << 1.0, 0.0, 1.1;
    // IMU data(x_vel, y_vel, yaw_vel)
    z << 1.0, 0.0, 0.9;

    // Set the measurement noise covariance matrix
    R.setIdentity();
    R *= 0.01;

    // Get measurement Jacobian matrix 
    H = ekf.observation_model();

    for(size_t i=0; i<10; i++)
    {
        ekf.predict(u, dt);
        ekf.correct(z, R, H);

        std::cout << "State: x = " << ekf.vecX()(0) << ", y = " << ekf.vecX()(1) << ", yaw = " << ekf.vecX()(2) << std::endl;
    }

    return 0;
}