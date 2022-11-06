#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>
#include <vector>


class Kmeans
{
    public:
        int k; //군집의 갯수
        int width, height;
        int pixel_size;
        int MAX_ = 500000;
        int mean_r, mean_g, mean_b;

        struct clu_struct
        {
            cv::Point3f tmp_clu;
            int lable;
        };
        clu_struct param;
        struct mean
        {
            int mean_r = 0;
            int mean_g = 0;
            int mean_b = 0;
        };
        mean mean_param;

        std::vector<cv::Point3f> src_points;
        std::vector<cv::Point3f> clu_center;
        std::vector<clu_struct> tmp_points;
        std::vector<mean> mean_index;

        cv::Mat src, res;
        
        void initValue(Kmeans *kmeans_)
        {
            kmeans_->src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/fruits.jpg");
            width = src.rows;
            height = src.cols;
            pixel_size = width*height;

            res.create(height, width, src.type());
        }

        void Kmeans_fun()
        {
            std::cout << "Insert Cluster?" << std::endl;
            std::cin >> k;
            for(int j = 0; j < width; j++)
                for(int i = 0; i <height; i++)                            //1. 모든 픽셀을 하나의 샘플 집합 X로 변환
                {
                    float x = src.at<cv::Vec3b>(j, i)[0];
                    float y = src.at<cv::Vec3b>(j, i)[1];
                    float z = src.at<cv::Vec3b>(j, i)[2];
                    src_points.push_back(cv::Point3f(x, y, z));
                }
            
            std::srand((unsigned int)time(NULL));
            for(int i = 0; i < k; i++)
            {
                int rand1, rand2, rand3;
                rand1 = rand() % 255;
                rand2 = rand() % 255;
                rand3 = rand() % 255;
                clu_center.push_back(cv::Point3f(rand1, rand2, rand3));    //2. clu center에 랜덤한 좌표
            }

            mean_index.push_back(mean_param);
            tmp_points.push_back(param);
            bool first_check = true;
            while(true)
            {

                for(int i = 0; i < pixel_size; i++)
                {
                    int distance;
                    int min_dist = MAX_;
                    int min_center = -1;

                    int x, y, z;
                    int xc, yc, zc;
                    for(int j = 0; j < k; j++)
                    {
                        x = src_points[i].x;
                        y = src_points[i].y;
                        z = src_points[i].z;
                        
                        xc = clu_center[j].x;
                        yc = clu_center[j].y;
                        zc = clu_center[j].z;

                        distance = sqrt(pow(x - xc, 2) + pow(y - yc, 2) + pow(z - zc, 2));
                        if(distance < min_dist)
                        {
                            min_dist = distance;
                            min_center = j;
                        }
                    }
                    tmp_points[i].lable = min_center;
                    tmp_points[i].tmp_clu = src_points[i];                              
                }
                if(first_check == true)
                {
                    first_check = false;
                    continue;
                }
                else if(first_check == false)
                    break;

                for(int i = 0; i < pixel_size; i++)
                {
                    for(int j = 0; j < k; j++)
                    {
                        if(tmp_points[i].lable == j)
                        {
                            mean_index[j].mean_r += tmp_points[i].tmp_clu.x;
                            mean_index[j].mean_g += tmp_points[i].tmp_clu.y;
                            mean_index[j].mean_b += tmp_points[i].tmp_clu.z;
                        }
                    }
                }
                for(int j = 0; j < k; j++)
                {
                    mean_index[j].mean_r = mean_index[j].mean_r / (pixel_size/3);
                    mean_index[j].mean_b = mean_index[j].mean_b / (pixel_size/3);
                    mean_index[j].mean_g = mean_index[j].mean_g / (pixel_size/3);
                }
            }
            for(int j = 0; j < pixel_size; j++)
            {
                for(int i = 0; i < k; i++)
                {
                    if(tmp_points[j].lable == i)
                    {

                    }
                }
            }
            
            // uchar *rimage = res.data;
            // for(int j = 0; j < clu_center.size(); j++)
            //     for(int i = 0; i < clu_center[j].size(); i++)
            //     {
            //         rimage[calc[j][i] * 3 + 0] = clu_cent[j].x;
            //         rimage[calc[j][i] * 3 + 1] = clu_cent[j].y;
            //         rimage[calc[j][i] * 3 + 2] = clu_cent[j].z;
            //     }




        }

    Kmeans()
    {
        initValue(this);
        Kmeans_fun();
    }
    ~Kmeans()
    {

    }

};

int main()r
{
    Kmeans kmeans_ = Kmeans();
}
