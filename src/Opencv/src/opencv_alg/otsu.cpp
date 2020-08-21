#include "opencv2/opencv.hpp"
#include <iostream>
#include <math.h>

class Otsu
{
public:
    int width, height;
    cv::Mat src, gray;
    cv::Mat histogram;
    cv::Mat otsu_image;
    double histo[256] = {
        0.0,
    };
    int pixel_size;
    float u_g;
    int l;

    cv::Point2d dispersion;

    Otsu()
    {
        intValue(this);
        make_histogram();
        otsu_algorithm();
        thresholding();
    }

    void intValue(Otsu *otsu)
    {
        otsu->src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/test2.jpg");
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        width = gray.rows;
        height = gray.cols;

        otsu->histogram = cv::Mat::ones(255,255,CV_8U);
        otsu->otsu_image.create(width, height, CV_8U);

        pixel_size = width*height;
    }

    void make_histogram()
    {
        for (int j = 0; j < width; j++)
        {
            for (int i = 0; i < height; i++)
            {
                int val = gray.at<uchar>(j, i);
                histo[val]++;
            }
        }

        for (int k = 0; k < 256; k++)
        {
            histo[k] = (histo[k] / pixel_size) * 256 * 10;
            line(histogram, cv::Point(k, 256), cv::Point(k, 256 - histo[k]), cv::Scalar(255), 1);
            //std::cout << histo[k] << std::endl;
        }
    }

    void otsu_algorithm()
    {
        l = sizeof(histo) / sizeof(double);
        int dispersion_value = 0;
        
        for (int i = 0; i < l - 1; i++)
        {
            u_g += i*histo[i];
        }
        for (int t1 = 1; t1 < l - 3; t1++)
        {
            float u0 = 0, w0 = 0;
            for (int i = 0; i <= t1; i++)
            {
                u0 += i * histo[i];
                w0 += histo[i];
            }
            w0 = 0 ? u0 = 0 : u0 = u0/w0;

            for(int t2 = t1 +1; t2 < l-2; t2++)
            {
                float u1=0,u2=0,w1=0,w2=0;
                for(int i =t1+1; i<= t2; i++)
                {
                    u1 += i*histo[i];
                    w1 += histo[i];
                }
                w1 = 0 ? u1 = 0 :  u1 = u1/w1;
                for(int i = t2+1; i <= l-1; i++)
                {
                    u2 = i * histo[i];
                    w2 = histo[i];
                }
                w2 = 0 ? u2 = 0 : u2 = u2 / w2;
                float dispersion_value_temp = w0 * pow(u0 - u_g, 2) + w1 * pow(u1 - u_g, 2) + w2 * pow(u2 - u_g, 2);
                if (dispersion_value < dispersion_value_temp)
                {
                    dispersion_value = dispersion_value_temp;

                    dispersion.x = t1;
                    dispersion.y = t2;
                }
            }
        }
        std::cout << dispersion.x << " " << dispersion.y << std::endl;
    }

    void thresholding()
    {
        for (int j = 0; j < width; j++)
            for (int i = 0; i < height; i++)
            {
                if (gray.at<uchar>(j, i) > dispersion.x)
                    otsu_image.at<uchar>(j, i) = 255;
                else if (gray.at<uchar>(j, i) > dispersion.y)
                    otsu_image.at<uchar>(j, i) = 127;
                else
                    otsu_image.at<uchar>(j, i) = 0;
            }
    }
};

int main()
{
    Otsu otsu = Otsu();

    cv::imshow("otsu_image", otsu.otsu_image);
    cv::waitKey(0);
    return 0;
}
