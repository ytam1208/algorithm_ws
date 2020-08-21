#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>


/* 문제
입력: 영상f(j, i), 0 <= j <= M-1, 
                   0 <= i <= N-1

출력: 삼진영상 g(j, i), 0 <= j <= M-1,
                        0 <= i <= N-1

    1. [알고리즘 2-1]을 이용하여 f의 정규 히스토그램을 만든다.
    2. for(t1 = 1 to L-3)
    3.  for(t2 = t1 + 1 to L-2)
    4.     식(5.2)를 이용하여 V(between)(t1, t2)를 계산한다.
    5. 2~4행에서 가장 큰 V(between)을 생성한 (t1, t2)를 임계값(t1, t2)로 취한다.
    6. (t1, t2)로 f를 삼진화하여 g를 만든다.
*/

class Otsu
{
    public:
        int width;
        int height;
        int pixel_size;
        int L;

        float ug;
        float w0, w1, w2;
        float u0, u1, u2;

        double histo[255] = {
            0.0
        };
        double histo_line[255] = {
            0.0
        };


        cv::Point2d t_point;

        cv::Mat src, gray;
        cv::Mat histrogram_grap, histo_equal, otsu_image, otsu_fun_image;

        //std::string path_img = "/home/cona/algorithm_ws/src/Opencv/data/detect_blob.png";
        std::string path_img = "/home/cona/algorithm_ws/src/Opencv/data/test4.png";

    Otsu()
    {
        runloop();
    }

    ~Otsu()
    {}

    void runloop()
    {
        imshow(this);
        make_histogram();
        Otsu_algo();
        thresholding();
    }

    void imshow(Otsu *otsu)
    {
        otsu->src = cv::imread(path_img);
        if(src.empty())
        {
            printf("No img check your img path!!");
            return;
        }

        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        // cv::imshow("color_img", src);
        cv::imshow("gray_img", gray);

        width = gray.rows;
        height = gray.cols;

        otsu->histrogram_grap = cv::Mat::ones(255, 255, CV_8U);
        otsu->otsu_image.create(width, height, CV_8U);
        otsu->otsu_fun_image.create(width, height, CV_8U);

        otsu->histo_equal.create(255, 255, CV_8U);
        cv::equalizeHist(gray, histo_equal);
        cv::imshow("histo_equal", histo_equal);

        pixel_size = width * height;
    }

    void make_histogram() // 1. 정규 히스토그램을 만든다.
    {        
        for(int j = 0; j < width - 1; j++)
        {
            for(int i = 0; i < height - 1; i++)
            {
                int val = gray.at<uchar>(j, i);
                histo[val]++;
                //printf("histo[%d] = %lf ", i, histo[i]);
            }
        }
        for(int i = 0; i < 256; i++)
        {
            histo[i] = (histo[i] / pixel_size);//정규 히스토그램
            histo_line[i] = histo[i] * 256 * 10; //히스토그램 그래프
            line(histrogram_grap, cv::Point(i, 256), cv::Point(i, 256 - histo_line[i]), cv::Scalar(255), 1); 
        }
        cv::imshow("histo_grap", histrogram_grap);
    }

    void Otsu_algo()
    {   
        L = sizeof(histo)/sizeof(double) + 1;       //histo의 갯수
        //printf("L = %d", L); 256
        int Vbetween = 0;
    
        for(int i = 0; i < L-1; i++)
        {
            ug += i * histo[i];                  //전체평균
        }
        std::cout << "ug = " << ug << std::endl;
        
        for(int t1 = 1; t1 < L-3; t1++)
        {
            w0 = 0, u0 = 0;
            for(int i = 0; i <= t1; i++)
            {
                float tmp;
                //printf("histo = %lf", histo[i]);

                w0 += histo[i];
                tmp += i * histo[i]; 
                u0 = tmp / w0;
            }
            if(w0 == 0)
                u0 = 0;
            else
                u0 = u0 / w0;
            // (w0 = 0) ? (u0 = 0) : (u0 = (u0 / w0));
            // std::cout << "w0 = " << w0 << " , " << "u0 = " << u0 << std::endl;

            for(int t2 = t1 + 1; t2 < L-2; t2++)
            {
                u1 = 0, u2 = 0, w1 = 0, w2 = 0;
                for(int i = t1+1; i <= t2; i++)
                {
                    float tmp;
                    w1 += histo[i];
                    tmp += i * histo[i];
                    u1 = tmp / w1;
                }
                if(w1 == 0)
                    u1 = 0;
                else
                    u1 = u1 / w1;
                // w1 = 0 ? u1 = 0 : u1 = u1 / w1;
                // std::cout << "w1 = " << w1 << "u1" << u1 << std::endl;

                for(int i = t2+1; i < L-1; i++)
                {
                    float tmp;
                    //printf("histo[%d] = %lf ", i, histo[i]);
                    w2 = histo[i];
                    tmp = i * histo[i];
                    u2 = tmp / w2;
                }
                if(w2 == 0)
                    u2 = 0;
                else
                    u2 = u2 / w2;
                //w2 = 0 ? u2 = 0 : u2 = u2 / w2;
                //std::cout << "w2 = " << w2 << " " << "u2 = " << u2 << std::endl;

                float Vbetween_tmp = 0;
                Vbetween_tmp = (w0 * pow(u0 - ug, 2)) + (w1 * pow(u1 - ug, 2)) + (w2 * pow(u2 - ug, 2));
                //std::cout << "Vbetween_tmp1 = " << Vbetween_tmp << std::endl;
                // std::cout << "w0 = " << w0 << " " << "u0 = " << u0 << " " <<std::endl;
                // std::cout << "w1 = " << w1 << " " << "u1 = " << u1 << " " <<std::endl;
                // std::cout << "w2 = " << w2 << " " << "u2 = " << u2 << " " <<std::endl;

                if(Vbetween < Vbetween_tmp)
                {
                    //std::cout << "Vbetween_tmp = " << Vbetween_tmp << std::endl;
                    Vbetween = Vbetween_tmp;
                    t_point.x = t1;
                    t_point.y = t2;
                }
            }
        }
                    std::cout << "t_point.x = " << t_point.x << " "<< "t_point.y = " << t_point.y << std::endl;
    }

    void thresholding()
    {
        for(int j = 0; j < width; j++)
            for(int i = 0; i < height; i++)
            {
                if(histo_equal.at<uchar>(j, i) > 0 && histo_equal.at<uchar>(j, i) < t_point.x)
                    otsu_image.at<uchar>(j, i) = 0;
                else if(histo_equal.at<uchar>(j, i) >= t_point.x && histo_equal.at<uchar>(j, i) < t_point.y)
                    otsu_image.at<uchar>(j, i) = 127;
                else
                    otsu_image.at<uchar>(j, i) = 255;
            }
    cv::imshow("Otsu_make_img", otsu_image);
    cv::threshold(gray, otsu_fun_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("Otsu_fun_img", otsu_fun_image);

    cv::waitKey(0);
    }
};


int main()
{
    Otsu otsu;
    return 0;
}