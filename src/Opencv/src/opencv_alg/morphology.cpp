#include "Opencv/Opencv_tuto.hpp"

int main()
{
    cv::Mat ori_src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/gray.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/gray.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat threshold;
    cv::Mat adaptive;
    cv::threshold(src, threshold, 150, 255, 0);
    cv::adaptiveThreshold(src, adaptive, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 10);
    cv::adaptiveThreshold(src, src, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 10);

    if(!src.data)
    {
        printf ("NO data\n");
        return 0;
    }
    cv::namedWindow("threshold_img", cv::WINDOW_AUTOSIZE);
    cv::imshow("threshold_img", threshold);

    cv::namedWindow("adaptive_img", cv::WINDOW_AUTOSIZE);
    cv::imshow("adaptive_img", adaptive);

    cv::namedWindow("binary_img", cv::WINDOW_AUTOSIZE);
    cv::imshow("binary_img", src);

    //영상 침식
    cv::Mat erode_img;
    cv::erode(ori_src, erode_img, cv::Mat());
    cv::namedWindow("erode_img", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("erode_img", 600, 0);
    cv::imshow("erode_img", erode_img);

    //영상 팽창
    cv::Mat dilated;
    cv::dilate(ori_src, dilated, cv::Mat());
    cv::namedWindow("dilated_img", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("dilated_img", 0, 600);
    cv::imshow("dilated_img", dilated);

    cv::Mat element5(5, 5, CV_8U, cv::Scalar(1));
    //모폴로지 close
    cv::Mat morph;
    cv::morphologyEx(src, morph, cv::MORPH_CLOSE, element5);
    cv::namedWindow("close_morphologyEx", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("close_morphologyEx", 600, 600);
    cv::imshow("close_morphologyEx", morph);

    //모폴로지 open
    cv::Mat morph2;
    cv::morphologyEx(src, morph2, cv::MORPH_OPEN, element5);
    cv::namedWindow("open_morphologyEx", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("open_morphologyEx", 1200, 600);
    cv::imshow("open_morphologyEx", morph2);

    //모폴로지 close & open
    cv::Mat morph3;
    cv::morphologyEx(src, morph3, cv::MORPH_CLOSE, element5);
    cv::morphologyEx(src, morph3, cv::MORPH_OPEN, element5);
    cv::namedWindow("close&open_morphologyEx", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("close&open_morphologyEx", 1200, 1200);
    cv::imshow("close&open_morphologyEx", morph3);

    //모폴로지 open & close
    cv::Mat morph4;
    cv::morphologyEx(src, morph4, cv::MORPH_OPEN, element5);
    cv::morphologyEx(src, morph4, cv::MORPH_CLOSE, element5);
    cv::namedWindow("close&open_morphologyEx", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("close&open_morphologyEx", 1800, 1200);
    cv::imshow("close&open_morphologyEx", morph3);


    cv::waitKey(0);
    return 0;
}