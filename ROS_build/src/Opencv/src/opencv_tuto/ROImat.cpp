#include "Opencv/OpencvBase.hpp"
//matrix addWeighted, copyTo tuto
int main()
{
    opencv_img img_;

    //addWeighted ROI방법
    //ROI 영역 초기화
    cv::Mat imageROI;
    imageROI = img_.src(cv::Rect(10, 10, img_.src2.cols, img_.src2.rows));

    //로고를 영상에 넣기
    cv::addWeighted(imageROI, 1.0, img_.src2, 0.3, 0., imageROI);

    //결과 보기
    cv::namedWindow("with logo");
    cv::imshow("with logo", img_.src);
    
    //copyTo ROI방법
    img_.src = cv::imread(img_.path, cv::IMREAD_COLOR);
    img_.src2 = cv::imread(img_.path2, cv::IMREAD_COLOR);

    //ROI 영역 초기화
    imageROI = img_.src(cv::Rect(10, 10, img_.src2.cols, img_.src2.rows));
    cv::Mat mask = cv::imread(img_.path2, cv::IMREAD_COLOR);

    //logo 이미지 이진화
    //cv::threshold(mask, mask, 150, 255, 0);

    //로고를 영상에 넣기
    img_.src2.copyTo(imageROI, mask);

    cv::namedWindow("copy logo");
    cv::imshow("copy logo", img_.src);


    cv::waitKey(0);
    return 0;
}