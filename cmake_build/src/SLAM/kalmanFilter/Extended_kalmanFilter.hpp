#ifndef __KALMAN_FILTER_HPP__
#define __KALMAN_FILTER_HPP__

#include <Eigen/Dense>
#include <cstdint>
#include <cmath>

namespace ekf
{
    using float32_t = float;

    template<size_t ROW, size_t COL>
    using Matrix = Eigen::Matrix<float32_t, ROW, COL>;

    template<size_t ROW>
    using Vector = Eigen::Matrix<float32_t, ROW, 1>;

    ///
    /// @brief DIM_X: dimension of state vector
    /// @brief DIM_Z: dimension of measurement vector
    ///
    template<size_t DIM_X, size_t DIM_Z> 
    class Extended_KalmanFilter
    {
    private:
        /// @brief estimated state vector
        Vector<DIM_X> m_vecX{ Vector<DIM_X>::Zero() };
        /// @brief state covariance matrix
        Matrix<DIM_X, DIM_X> m_matP{ Matrix<DIM_X, DIM_X>::Zero() };  
        /// @brief motion noise covariance matrix
        Matrix<DIM_X, DIM_X> Q;

    public:
        Extended_KalmanFilter()
        {
            Q.setIdentity();
            Q *= 0.01;      // Adjust this value based on your system
        }
        ~Extended_KalmanFilter(){}

        Vector<DIM_X>& vecX() {return m_vecX;}
        const Vector<DIM_X>& vecX() const {return m_vecX;} 
        
        Matrix<DIM_X, DIM_X>& matP() {return m_matP;}
        const Matrix<DIM_X, DIM_X>& matP() const {return m_matP;}

        ///
        /// @brief velocity motion model (2-wheel robot)
        /// @param velocity linear_vel, angular_vel
        /// @param dt 
        ///
        void motion_model(const Vector<DIM_X>& velocity, const float32_t dt)
        {
            float32_t x_vel = velocity(0);  // Linear velocity 
            float32_t yaw_vel = velocity(2); // Angular velocity (yaw rate)

            float32_t x = m_vecX(0);
            float32_t y = m_vecX(1);
            float32_t yaw = m_vecX(2);

            // Update the state using the velocity and time step
            float32_t new_x = x + (x_vel * std::cos(yaw)) * dt;
            float32_t new_y = y + (x_vel * std::sin(yaw)) * dt;
            float32_t new_yaw = yaw + yaw_vel * dt;

            // Optionally, normalize yaw to keep it within [-pi, pi]
            new_yaw = std::atan2(std::sin(new_yaw), std::cos(new_yaw));

            m_vecX(0) = new_x;
            m_vecX(1) = new_y;
            m_vecX(2) = new_yaw;
        }

        ///
        /// @brief observation model
        /// @return measurement noise covariance H matrix
        ///
        Matrix<DIM_Z, DIM_X> observation_model(void)
        {
            /*
                Why observation model is identity matrix? 
                because the measurement is the same as the state vector (x, y, yaw) = (x, y, yaw)
                If the measurement is different from the state vector, you need to define the observation model matrix H
            */
            Matrix<DIM_Z, DIM_X> H;
            H.setIdentity();
            return H;
        }        

        ///
        /// @brief Prediction step
        /// @param velocity linear_vel, angular_vel
        /// @param dt 
        ///
        void predict(const Vector<DIM_X>& velocity, const float32_t dt)
        {
            motion_model(velocity, dt);

            // State transition matrix
            Matrix<DIM_X, DIM_X> F;
            F.setIdentity();
            F(0, 2) = -velocity(0) * std::sin(m_vecX(2)) * dt;
            F(1, 2) = velocity(0) * std::cos(m_vecX(2)) * dt;

            m_matP = F * m_matP * F.transpose() + Q;
        }

        ///
        /// @brief correct state of with a linear measurement model. 
        /// @param vecZ: measurement of model
        /// @param matR: measurement noise covariance matrix of model
        /// @param matH: measurement matrix of model
        ///
        void correct(const Vector<DIM_Z>& vecZ, const Matrix<DIM_Z, DIM_Z>& matR, const Matrix<DIM_Z, DIM_X>& matH)
        {
            const Matrix<DIM_X, DIM_X> matI{ Matrix<DIM_X, DIM_X>::Identity() };             // Identity matrix
            const Matrix<DIM_Z, DIM_Z> matSt{ matH * m_matP * matH.transpose() + matR };     // Innovation covariance
            const Matrix<DIM_X, DIM_Z> matKt{ m_matP * matH.transpose() * matSt.inverse() }; // Kalman gain

            // Update all state variables (x, y, yaw)
            m_vecX = m_vecX + matKt * (vecZ - matH * m_vecX);
            m_matP = (matI - matKt * matH) * m_matP;
        }
    };
}

#endif