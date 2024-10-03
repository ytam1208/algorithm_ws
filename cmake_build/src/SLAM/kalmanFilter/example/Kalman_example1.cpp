#include <iostream>
#include "../KalmanFilter.hpp"

/*
    [15-D] State vector: IMU (position, velocity, orientation, accelerometer_bias, gyroscope_bias)
    [3-D]  State vector: 2 wheel differential (position(x,y), orientation(yaw))

    This example is a 1D Kalman filter example. 
    The state vector is a 2-D vector, and the measurement vector is a 1-D vector.
*/

static constexpr size_t DIM_X{ 2 };         // dimension of state vector
static constexpr size_t DIM_Z{ 1 };         // dimension of measurement vector
static constexpr kf::float32_t T { 1.0 };   // time step(ex. 100hz -> 0.01)
static constexpr kf::float32_t Q11{ 0.1 };  // motion model noise covariance
static constexpr kf::float32_t Q22{ 0.1 };  // motion model noise covariance
static kf::KalmanFilter<DIM_X, DIM_Z> KalmanFilter;

void executePredictionStep();
void executeCorrectionStep();

int main(int argc, char** argv)
{
    executePredictionStep();
    executeCorrectionStep();

    return 0;
}

void executePredictionStep()
{
    KalmanFilter.vecX() << 0.0, 2.0;            // initial state vector
    KalmanFilter.matP() << 0.1, 0.0, 0.0, 0.1;  // initial covariance matrix

    kf::Matrix<DIM_X, DIM_X> F;                 // state transition matrix
    F << 1.0, T, 0.0, 1.0;

    kf::Matrix<DIM_X, DIM_X> Q;                 // process noise covariance matrix
    Q(0, 0) = (Q11 * T) + (Q22 * (std::pow(T, 3) / 3.0));
    Q(0, 1) = Q(1, 0) = (Q22 * (std::pow(T, 2) / 2.0));
    Q(1, 1) = Q22 * T;

    KalmanFilter.predict(F, Q);                // predict state

    std::cout << "\npredicted state vector = \n" << KalmanFilter.vecX() << "\n";
    std::cout << "\npredicted state covariance = \n" << KalmanFilter.matP() << "\n";
}

void executeCorrectionStep()
{
    kf::Vector<DIM_Z> vecZ;                             // measurement vector
    vecZ << 2.25;

    kf::Matrix<DIM_Z, DIM_Z> matR;                      // measurement noise covariance matrix
    matR << 0.01;

    kf::Matrix<DIM_Z, DIM_X> matH;                       // measurement matrix
    matH << 1.0, 0.0;

    KalmanFilter.correct(vecZ, matR, matH);              // correct state

    std::cout << "\ncorrected state vector = \n" << KalmanFilter.vecX() << "\n";
    std::cout << "\ncorrected state covariance = \n" << KalmanFilter.matP() << "\n";
}