#include <iostream>
#include <Eigen/Dense>

int main(int argc, char** argv)
{
    Eigen::Matrix<double, 2, 2> A;
    A << 1, 2, 2, 3;
    std::cout << "Here is the matrix A: \n" << A << std::endl;

    Eigen::EigenSolver<Eigen::Matrix<double, 2, 2>> eigenSolver(A);

    auto eigenvalueMatrix = eigenSolver.eigenvalues();
    auto eigenvectors_Matrix = eigenSolver.eigenvectors();    //eigenvectors Matrix

    std::cout << "The eigenvalues of A are: \n" << eigenvalueMatrix << std::endl;
    std::cout << "Here`s a matrix whose colums are eigenvectors of A corresponding to these eigenvalues: \n" <<
                 eigenvectors_Matrix << std::endl;
    
    Eigen::Matrix<std::complex<double>, 2, 2> eigenvectors_complex = eigenSolver.eigenvectors();  //Rotation
    Eigen::Matrix<double, 2, 2> eigenvectors_real = eigenvectors_complex.real();     // 복소수 형식의 고유벡터 행렬을 실수 형식으로 변환    
    
    Eigen::Matrix<std::complex<double>, 2, 1> eigenvalues_complex = eigenSolver.eigenvalues();  // 복소수 형식의 고유값 행렬
    Eigen::Matrix<double, 2, 1> eigenvalues_real = eigenvalues_complex.real(); // 복소수 형식의 고유값 행렬을 실수 형식으로 변환
    
    /* Eigendecomposition calculate */
    Eigen::Matrix<double, 2, 2> Lambda_Matrix;
    Lambda_Matrix(0,0)= eigenvalues_real(0);
    Lambda_Matrix(0,1)= 0.f;
    Lambda_Matrix(1,0)= 0.f;
    Lambda_Matrix(1,1)= eigenvalues_real(1);

    Eigen::Matrix<double, 2,2> result_A = eigenvectors_real * Lambda_Matrix * eigenvectors_real.inverse();
    std::cout << "Eigendecompostion A = \n" << result_A << std::endl;
    /* Eigendecomposition calculate */

    return 0;
}