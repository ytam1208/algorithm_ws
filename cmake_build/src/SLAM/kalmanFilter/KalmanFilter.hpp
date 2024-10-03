#ifndef __KALMAN_FILTER_HPP__
#define __KALMAN_FILTER_HPP__

#include <Eigen/Dense>
#include <cstdint>

namespace kf
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
class KalmanFilter
{
public:
    KalmanFilter(){}
    ~KalmanFilter(){}

    Vector<DIM_X>& vecX() {return m_vecX;}
    const Vector<DIM_X>& vecX() const {return m_vecX;} 
    
    Matrix<DIM_X, DIM_X>& matP() {return m_matP;}
    const Matrix<DIM_X, DIM_X>& matP() const {return m_matP;}
    
    ///
    /// @brief predict state with a linear process model.
    /// @param matF: state transition matrix
    /// @param matQ: process noise covariance matrix
    ///
    void predict(const Matrix<DIM_X, DIM_X>& matF, const Matrix<DIM_X, DIM_X>& matQ)
    {
        m_vecX = matF * m_vecX;
        m_matP = matF * m_matP * matF.transpose() + matQ;
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

        m_vecX = m_vecX + matKt * (vecZ - matH * m_vecX);
        m_matP = (matI - matKt * matH) * m_matP;
    }

private:
    /// @brief estimated state vector
    Vector<DIM_X> m_vecX{ Vector<DIM_X>::Zero() };
    /// @brief state covariance matrix
    Matrix<DIM_X, DIM_X> m_matP{ Matrix<DIM_X, DIM_X>::Zero() };  
};
}

#endif