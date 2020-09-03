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
        std::string path_img = "/home/cona/algorithm_ws/src/Opencv/data/test5.png";

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
        cv::imshow("src_img", src);
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
                printf("val = %d\n", val);
                histo[val]++; //그레이 스케일에서 값들의 갯수를 저장한다.
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
        L = sizeof(histo)/sizeof(double);       //histo의 갯수
        int Vbetween = 0;

        for(int i = 0; i < L-1; i++)
        {
            ug += i * histo[i];                  //전체평균
        }
        
        for(int t1 = 1; t1 < L-3; t1++)
        {
            w0 = 0, u0 = 0;
            float tmp1 = 0;
            for(int i = 0; i <= t1; i++)
            {
                w0 += histo[i];
                tmp1 += i * histo[i]; 
            }
            if(w0 == 0)
                u0 = 0;
            else
                u0 = tmp1 / w0;

            for(int t2 = t1 + 1; t2 < L-2; t2++)
            {
                float tmp2 = 0;
                u1 = 0;
                u2 = 0;
                w1 = 0;
                w2 = 0;
                for(int i = t1+1; i <= t2; i++)
                {
                    w1 += histo[i];
                    tmp2 += i * histo[i];
                }
                if(w1 == 0)
                    u1 = 0;
                else
                    u1 = tmp2 / w1;

                float tmp3 = 0;
                for(int i = t2+1; i < L-1; i++)
                {
                    w2 = histo[i];
                    tmp3 += i * histo[i];
                }
                if(w2 == 0)
                    u2 = 0;
                else
                    u2 = tmp3 / w2;

                float Vbetween_tmp = 0;
                Vbetween_tmp = (w0 * pow(u0 - ug, 2)) + (w1 * pow(u1 - ug, 2)) + (w2 * pow(u2 - ug, 2));

                if(Vbetween < Vbetween_tmp)
                {
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
                if(gray.at<uchar>(j, i) <= t_point.x)
                    otsu_image.at<uchar>(j, i) = 0;
                else if(t_point.x < gray.at<uchar>(j, i) && gray.at<uchar>(j, i) <= t_point.y)
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