#include <iostream>
#include <Eigen/Dense>

int main() {
    Eigen::MatrixXd data(3, 2);
    data << 0.5, 0, 1.5, 0.0, -0.5, 1;

    // 공분산 행렬 계산
    Eigen::MatrixXd cov_matrix = data.transpose() * data / (data.rows() - 1);

    // 고유값과 고유벡터 계산
    Eigen::EigenSolver<Eigen::MatrixXd> solver(cov_matrix);
    
    // 정규 벡터 출력
    Eigen::VectorXd eigenvector1 = solver.eigenvectors().col(0).real().normalized();
    std::cout << "Normal Vector: \n" << eigenvector1 << std::endl;
    Eigen::VectorXd eigenvector2 = solver.eigenvectors().col(1).real().normalized();
    std::cout << "Normal Vector: \n" << eigenvector2 << std::endl;

    Eigen::MatrixXd eigenvector3 = solver.eigenvectors().real().col(solver.eigenvectors().real().cols() - 1);
    std::cout << "Normal Vector: \n" << eigenvector3 << std::endl;
    // 상관 계수 계산
    double correlation_coefficient = cov_matrix(0, 1) / std::sqrt(cov_matrix(0, 0) * cov_matrix(1, 1));
    if(cov_matrix(0, 1) == 0.0)correlation_coefficient=0.0f;

    // 정규 벡터 계산
    Eigen::VectorXd normal_vector(2);
    normal_vector << -correlation_coefficient / std::sqrt(1 + std::pow(correlation_coefficient, 2)),
                    1 / std::sqrt(1 + std::pow(correlation_coefficient, 2));
    
    

    std::cout << "Normal Vector: \n" << normal_vector << std::endl;
    return 0;
}