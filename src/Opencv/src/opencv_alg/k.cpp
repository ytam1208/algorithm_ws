#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

class K_means
{
    public:

        int k;
        int width, height, pixel_size;
        int MSG_size;
        int r, g, b;
        int cr, cg, cb;
        int mean_r, mean_g, mean_b;

        struct lable_
        {
            int lable1;
            int r;
            int g;
            int b;
        };
        lable_ param;
        std::vector<lable_> tmp_clu_points;      //개별 군집과 가까운 RGB컨테이너 배열의 위치를 lable 별로 저장

        struct mean_struct
        {
            int mean_r = 0;
            int mean_b = 0;
            int mean_g = 0;
        };
        mean_struct mean;
        std::vector<mean_struct> mean_value;           //tmp_clu에서 lable별 rgb들의 평균을 저장

        cv::Mat src;
        cv::Mat dst;

        std::vector<cv::Point3f> src_points;           //이미지에서의 RGB 컨테이너
        std::vector<cv::Point3f> clu_points;           //임의의 군집 좌표
        K_means()
        {
            initvalue(this);
            MSG();
            IGG();
            runloop();
            make_kmeans();

        }
        ~K_means()
        {

        }
        void make_kmeans()
        {
            int point_size = 0;
            int k_ = 0;
                for(int j = 0; j < width; j++)
                    for(int i = 0; i < height; i++)
                    {
                        if(k_ == k)
                            k_ = 0;
                        if(point_size > pixel_size)
                        {
                            printf("point_size = %d", point_size);
                            printf("out!\n");
                            goto EXIT;
                        }
                        if(k_ == tmp_clu_points[point_size].lable1)
                        {
                            // printf("go\n");
                            int r1 = tmp_clu_points[point_size].r;
                            int g1 = tmp_clu_points[point_size].g;
                            int b1 = tmp_clu_points[point_size].b;
                            dst.at<cv::Vec3b>(j,i)[0] = r1;
                            dst.at<cv::Vec3b>(j,i)[1] = g1;
                            dst.at<cv::Vec3b>(j,i)[2] = b1;
                        }
                        point_size++;
                        k_++;
                    }
            EXIT:
                cv::imshow("Result_img", dst);
                cv::waitKey(0);
        }
        void runloop() 
        {
            bool first_check = true;
            int distance;
            int min_dist = 500000;
            int complete_k = -1;

            while(true)
            {
                // printf("pixel_size = %d\n", pixel_size);
                bool check_distane = false;
                for(int j = 0; j < pixel_size; j++)
                {
                    tmp_clu_points.push_back(param);
                    for(int i = 0; i < k; i++)
                    {
                        cr = clu_points[i].x;
                        cg = clu_points[i].y;
                        cb = clu_points[i].z;

                        r = src_points[j].x;
                        g = src_points[j].y;
                        b = src_points[j].z;

                        distance = sqrt(pow(r - cr, 2) + pow(g - cg, 2) + pow(b - cb, 2));
                        if(distance < min_dist)
                        {
                            min_dist = distance;
                            complete_k = i;
                            check_distane = true;
                        }
                    }
                    if(check_distane == true)  //3. 군집과 가장 가까운 src_points, 몇 번째 군집인지를 저장한다.
                    {
                        tmp_clu_points[j].lable1 = complete_k;
                        tmp_clu_points[j].r = r;
                        tmp_clu_points[j].g = g;
                        tmp_clu_points[j].b = b;
                    }
                }
                // int count0 = 0, count1 = 0, count2 = 0;
                // for(int i = 0; i < pixel_size; i++)
                // {
                //     printf("i = %d\n", i);
                //     printf("tmp_clu_points.lable = %d\n", tmp_clu_points[i].lable1);
                //     if(tmp_clu_points[i].lable1 == 0)
                //     {
                //         count0++;
                //     }
                //     else if(tmp_clu_points[i].lable1 == 1)
                //     {
                //         count1++;
                //     }
                //     else if(tmp_clu_points[i].lable1 == 2)
                //     {
                //         count2++;
                //     }
                // }
                // printf("pixel_size = %d\n",pixel_size);
                // printf("count0 = %d, count1 = %d, count2 = %d \n", count0, count1, count2);
                if(first_check == false)
                    break;
                else if(first_check == true)
                    first_check = false;

                //4. 군집들 중에 가장 가까운 src_points들의 평균을 구한다.
                mean_value.push_back(mean);
                for(int j = 0; j < k; j++)
                {
                    int tmp_r = 0, tmp_g = 0, tmp_b = 0;
                    for(int i = 0; i < pixel_size; i++)
                    {
                        if(j == tmp_clu_points[i].lable1) 
                        {
                            tmp_r += tmp_clu_points[i].r;
                            tmp_g += tmp_clu_points[i].g;
                            tmp_b += tmp_clu_points[i].b;                            
                        }
                    }
                    if(tmp_r == 0)
                        tmp_r = 0;
                    else if(tmp_g == 0)
                        tmp_g = 0;
                    else if(tmp_b == 0)
                        tmp_b = 0; 
                    else{
                        tmp_r = tmp_r/pixel_size;
                        tmp_g = tmp_g/pixel_size;
                        tmp_b = tmp_b/pixel_size;
                    }
                    mean_value[j].mean_r = tmp_r;
                    mean_value[j].mean_g = tmp_g;
                    mean_value[j].mean_b = tmp_b;
                }
                for(int i = 0; i < k; i++)
                    for(int j = 0; j < pixel_size; j++)
                    {
                        tmp_clu_points[j].r = mean_value[i].mean_r;
                        tmp_clu_points[j].g = mean_value[i].mean_g;
                        tmp_clu_points[j].b = mean_value[i].mean_b;                            
                    }
            }
        }
        void IGG() //Make Clu group    2.k개의 군집 중심을 초기화한다.(랜덤으로 k개의 좌표를 생성해준다)
        {
            srand((unsigned int)time(NULL));
            for(int i = 0; i < k; i++)
            {
                int rand1, rand2, rand3;
		        rand1 = rand() % 255, rand2 = rand() % 255, rand3 = rand() % 255;
                clu_points.push_back(cv::Point3f(rand1, rand2, rand3));
                // printf("rand1 = %d, rand2 = %d, rand3 = %d\n", rand1, rand2, rand3);
            }
        }
        void MSG() //Make Sample group 1.화소를 X = (R,G,B) 형태로 변환하여 샘플 집합 X 컨테이너로 구성한다.
        {
            for(int j = 0; j < width; j++)
                for(int i = 0; i < height; i++)
                {
                    uchar r_ = src.at<cv::Vec3b>(j,i)[0];
                    uchar g_ = src.at<cv::Vec3b>(j,i)[1];
                    uchar b_ = src.at<cv::Vec3b>(j,i)[2];

                    src_points.push_back(cv::Point3f(r_, g_, b_));
                }
            MSG_size = src_points.size();
            // for(int i = 0; i < pixel_size; i++)
            //     printf("src_points.x = %lf, src_points.y = %lf, src_point.z = %lf \n", src_points[i].x, src_points[i].y, src_points[i].z);
        }
        void initvalue(K_means *k_means)
        {
            k_means->src = cv::imread("/home/cona/algorithm_ws/src/Opencv/data/home.jpg");
            width = src.rows;
            height = src.cols;
            pixel_size = width * height;
            dst = cv::Mat::zeros(width, height, CV_8UC3);

            std::cout << "Insert K!" << std::endl;
            std::cin >> k;
            cv::imshow("Input_img", src);
        }
};

int main()
{
    K_means k_means = K_means();
    return 0;
}