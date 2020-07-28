#include "opencv2/opencv.hpp"
#include <iostream>


int main()
{
    cv::VideoCapture cap("../data/kim.avi");

    if(!cap.isOpened()){
        std::cerr << "Video open failed" << std::endl;
        return -1;
    }

    cv::HOGDescriptor hog;
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());

    cv::Mat frame;
    while(true){
        cap >> frame;
        if (frame.empty())
            break;

        std::vector<cv::Rect> detected;
        hog.detectMultiScale(frame, detected);

        for (cv::Rect r : detected){
            cv::Scalar c = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
            rectangle(frame, r, c, 3);
        }

        cv::imshow("frame", frame);

        if(cv::waitKey(10) == 27)
            break;
    }

    //return 0;
 }
