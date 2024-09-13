#include <iostream>
#include <Opencv/Opencv.hpp>
#include <vector>

void draw_data(std::vector<cv::Point2d>& left, std::vector<cv::Point2d>& right, std::vector<cv::Point2d>& new_data)
{
    cv::Mat left_img = cv::Mat::zeros(500,500,CV_8UC3);
    cv::rectangle(left_img, cv::Point(0,0), cv::Point(500,500), cv::Scalar(255,255,255), 1);
    cv::Mat right_img = cv::Mat::zeros(500,500,CV_8UC3);
    cv::rectangle(right_img, cv::Point(0,0), cv::Point(500,500), cv::Scalar(255,255,255), 1);
    
    for(int i=0; i<left.size(); i++)
    {
        cv::circle(left_img, left[i], 1, cv::Scalar(0,255,0), -1);
    }
    for(int i=0; i<right.size(); i++)
    {
        cv::circle(right_img, right[i], 1, cv::Scalar(0,255,0), -1);
    }

    for(int i=0; i<new_data.size(); i++)
    {
        cv::circle(left_img, new_data[i], 1, cv::Scalar(0,0,255), -1);
    }
    for(int i=0; i<new_data.size(); i++)
    {
        cv::circle(right_img, new_data[i], 1, cv::Scalar(0,0,255), -1);
    }    
    cv::Mat merge;
    cv::hconcat(left_img, right_img, merge);
    cv::imshow("result", merge);
    cv::waitKey(0);
}

cv::Mat Get_Homogeneous(std::vector<cv::Point2d>& points)
{
    cv::Mat data = cv::Mat(3, points.size(), CV_64F);
    for(int i=0; i<points.size(); i++)
    {
        data.at<double>(0,i) = points[i].x;
        data.at<double>(1,i) = points[i].y;
        data.at<double>(2,i) = 1.0;
    }
    return data;
}

cv::Mat Matrix_multiplication(cv::Mat& p, cv::Mat& p_prime)
{
    cv::Mat output = cv::Mat(9, p.cols, CV_64F);
    
    for(int i=0; i<8; i++)
    {
        cv::Mat row_p = p.col(i);
        cv::Mat row_p_prime = p_prime.col(i);
        
        cv::Mat multiplication_row = row_p_prime * row_p.t();
        multiplication_row = multiplication_row.reshape(1, 9); //호모지니어스(1)
        multiplication_row.copyTo(output.col(i));
    }
    return output;
}

cv::Mat Transform(cv::Mat& data, cv::Mat& RT)
{
    cv::Mat output = data.clone();
    for(int i=0; i<data.cols; i++)
    {
        cv::Mat vector = data.col(i);
        cv::Mat trans = RT * vector;
        trans.copyTo(output.col(i));
    }
    return output;
}

cv::Mat Eight_point(std::vector<cv::Point2d>& left, std::vector<cv::Point2d>& right)
{
    cv::Mat p = Get_Homogeneous(left);
    cv::Mat p_prime = Get_Homogeneous(right);

    cv::Mat multi = Matrix_multiplication(p, p_prime);
    cv::Mat W, U, VT; //singular vec, left singular vec, transposed matrix of right singular vec
    cv::SVD::compute(multi.clone(), W, U, VT, cv::SVD::FULL_UV);
    // std::cout << "W is " << W.size() << " of type " << W.type() << std::endl;
    // std::cout << "U is " << U.size() << " of type " << U.type() << std::endl;
    // std::cout << "VT is " << VT.size() << " of type " << VT.type() << std::endl;    
    // std::cout << "W is \n" << W << std::endl;
    // std::cout << "U is \n" << U << std::endl;
    // std::cout << "VT is \n" << VT << std::endl;

    cv::Mat F_vec = VT.col(VT.cols - 1);
    // std::cout << "F vec = \n " << F_vec << std::endl;

    cv::Mat F = cv::Mat::zeros(3,3,CV_64F);
    for(int i=0; i<9; i++)
    {
        F.at<double>(i) = F_vec.at<double>(i);    
    }

    cv::SVD::compute(F.clone(), W, U, VT, cv::SVD::FULL_UV);
    
    //Setting The Smallest Eigen Value to Zero
    W.at<double>(W.rows - 1, 0) = 0.0;

    cv::Mat sigmaMatrix = cv::Mat::zeros(U.cols, VT.rows, CV_64F);
    for(int i = 0; i < W.rows; i++)
    {
        sigmaMatrix.at<double>(i, i) = W.at<double>(i, 0);    
    }

    cv::Mat NewF = U * sigmaMatrix * VT;
    std::cout << "F vec = \n " << NewF << std::endl;
    
    cv::Mat output = Transform(p, NewF);
    return output;
}


int main(int argc, char** argv)
{
    double points_left_tmp[16] = {10, 20, 30, 50, 60, 80, 90, 120, 130, 170, 180, 230, 240, 300, 310, 380};
    double points_right_tmp[16] = {20, 30, 40, 60, 70, 90, 100, 130, 140, 180, 190, 240, 250, 310, 320, 390};    

    std::vector<cv::Point2d> points_left, points_right;
    for(int i = 0; i < 8; i++)
    {
        cv::Point2d p1(points_left_tmp[2 * i], points_left_tmp[2 * i + 1]);
        cv::Point2d p2(points_right_tmp[2 * i], points_right_tmp[2 * i + 1]);
        points_left.push_back(p1); 
        points_right.push_back(p2); 
    }
    cv::Mat refine_data = Eight_point(points_left, points_right);
    
    std::vector<cv::Point2d> refine;
    for(int i=0; i<refine_data.cols; i++)
    {
        cv::Point2d p(refine_data.at<double>(i,0)*10, refine_data.at<double>(i,1)*10);
        refine.push_back(p);
    }

    draw_data(points_left, points_right, refine);
    return 0;
}