#include "opencv2/opencv.hpp"
#include <iostream>

class Harris
{
public:
    int width, height;
    cv::Mat src, gray, dx2, dy2, dxy, gdx2, gdy2, gdxy, crf;
    float gaussian[5][5] = {{1, 4, 6, 4, 1}, {4, 16, 24, 16, 4}, {6, 24, 36, 24, 6}, {4, 16, 24, 16, 4}, {1, 4, 6, 4, 1}};
    float thred;
    std::vector<cv::Point2d> corners;

    Harris()
    {
        intValue(this);
        differential();
        fliter_gaussian();
        count_possiblitity();
        decition_corner_point();
        drawing();
    }

    void intValue(Harris *harris)
    {
        harris->src = cv::imread("../data/corner.png");
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

        width = gray.rows;
        height = gray.cols;

        harris->dx2.create(width, height, CV_32FC3);
        harris->dy2.create(width, height, CV_32FC3);
        harris->dxy.create(width, height, CV_32FC3);
        
        harris->gdx2.create(width, height, CV_32FC3);
        harris->gdy2.create(width, height, CV_32FC3);
        harris->gdxy.create(width, height, CV_32FC3);
        
        harris->crf.create(width, height, CV_32FC3);

        thred = 0.1;

        for (int y = 0; y < 5; y++)
            for (int x = 0; x < 5; x++)
            {
                gaussian[y][x] /= 256.f;
            }
    }

    void differential()
    {
        float tx, ty;

        for (int j = 1; j < width - 1; j++)
            for (int i = 1; i < height - 1; i++)
            {
                tx = (gray.at<uchar>(j - 1, i + 1) + gray.at<uchar>(j, i + 1) + gray.at<uchar>(j + 1, i + 1) - gray.at<uchar>(j - 1, i - 1) - gray.at<uchar>(j, i - 1) - gray.at<uchar>(j + 1, i - 1)) / 6.f;
                ty = (gray.at<uchar>(j + 1, i - 1) + gray.at<uchar>(j + 1, i) + gray.at<uchar>(j + 1, i + 1) - gray.at<uchar>(j - 1, i - 1) - gray.at<uchar>(j - 1, i) - gray.at<uchar>(j - 1, i + 1)) / 6.f;

                dx2.at<float>(j, i) = tx * tx;
                dy2.at<float>(j, i) = ty * ty;
                dxy.at<float>(j, i) = tx * ty;
            }
    }

    void fliter_gaussian()
    {
        float tx2, ty2, txy;
        for (int j = 2; j < width - 2; j++)
            for (int i = 2; i < height - 2; i++)
            {
                tx2 = ty2 = txy = 0;
                for (int y = 0; y < 5; y++)
                    for (int x = 0; x < 5; x++)
                    {
                        tx2 += (dx2.at<float>(j + y - 2, i + x - 2) * gaussian[y][x]);
                        ty2 += (dy2.at<float>(j + y - 2, i + x - 2) * gaussian[y][x]);
                        txy += (dxy.at<float>(j + y - 2, i + x - 2) * gaussian[y][x]);
                    }
                gdx2.at<float>(j, i) = tx2;
                gdy2.at<float>(j, i) = ty2;
                gdxy.at<float>(j, i) = txy;
            }
    }

    void count_possiblitity()
    {
        float k = 0.04f;
        for (int j = 2; j < width - 2; j++)
            for (int i = 2; i < height - 2; i++)
            {
                crf.at<float>(j, i) = (gdx2.at<float>(j, i) * gdy2.at<float>(j, i) - gdxy.at<float>(j, i) * gdxy.at<float>(j, i) - k * (gdx2.at<float>(j, i) + gdy2.at<float>(j, i)) * (gdx2.at<float>(j, i) + gdy2.at<float>(j, i)));
            }
    }

    void decition_corner_point()
    {
        corners.clear();
        float value = 0.0;

        for (int j = 2; j < width - 2; j++)
            for (int i = 2; i < height - 2; i++)
            {
                value = crf.at<float>(j, i);
                //std::cout << "value " <<value << std::endl;
                if (value > thred)
                {
                    if (value > crf.at<float>(j - 1, i) && value > crf.at<float>(j - 1, i + 1) &&
                        value > crf.at<float>(j, i + 1) && value > crf.at<float>(j + 1, i + 1) &&
                        value > crf.at<float>(j + 1, i) && value > crf.at<float>(j + 1, i - 1) &&
                        value > crf.at<float>(j, i - 1) && value > crf.at<float>(j - 1, i - 1))
                    {
                        cv::Point2d point(j, i);
                        corners.push_back(point);
                    }
                }
            }
    }

    void drawing()
    {
        for (int i = 0; i < corners.size(); i++)
        {
            cv::circle(src, cv::Point(corners[i].x, corners[i].y), 2, cv::Scalar(0, 0, 255), -1);
            //std::cout << "x " <<corners[i].x<< " y " <<corners[i].y << std::endl;
        }
    }
};


int main()
{
    Harris harris = Harris();

    cv::imshow("src", harris.src);
    cv::waitKey(0);
    return 0;
}
