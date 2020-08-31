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
        int tmp_r, tmp_g, tmp_b;

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
        std::vector<lable_> tmp_clu2_points;      

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
            int k_ = 0;
            int point_size = 0;
            bool check_flag = true;
            while(check_flag)
            {
                if(point_size >= pixel_size)
                {
                    check_flag = false;
                    continue;
                }
                for(int j = 0; j < width; j++)
                    for(int i = 0; i < height; i++)
                    {
                        if(k_ > k)
                            k_ = 0;
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
            }
            cv::imshow("Result_img", dst);
            cv::waitKey(0);
        }
        void runloop() 
        {
            int distance;
            int check_complete_k;
            bool is_first = true;
            bool stop_flag = false;

            while(true)
            {
                // printf("pixel_size = %d\n", pixel_size);
                for(int j = 0; j < pixel_size; j++)
                {
                    int min_dist = 500000;
                    int complete_k = -1;

                    for(int i = 0; i < k; i++)
                    {
                        r = src_points[j].x;
                        g = src_points[j].y;
                        b = src_points[j].z;

                        cr = clu_points[i].x;
                        cg = clu_points[i].y;
                        cb = clu_points[i].z;
                        distance = sqrt(pow(r - cr, 2) + pow(g - cg, 2) + pow(b - cb, 2));
                        if(distance < min_dist)
                        {
                            min_dist = distance;
                            complete_k = i; //가장 가까운 군집 k를 기억
                        }
                    }
                    //3. 군집과 가장 가까운 src_points, 몇 번째 군집인지를 저장한다.
                    lable_ param;
                    param.lable1 = complete_k;
                    param.r = r;
                    param.g = g;
                    param.b = b;
                    tmp_clu_points.push_back(param);
                }
                if(is_first == true)
                    is_first = false;

                else if(is_first == false)
                {
                    for(int i = 0; i < pixel_size; i++)
                    {
                        if(tmp_clu_points[i].r == tmp_clu2_points[i].r 
                        && tmp_clu_points[i].b == tmp_clu2_points[i].b 
                        && tmp_clu_points[i].g == tmp_clu2_points[i].g)
                        {
                            stop_flag = true;
                        }
                    }
                }
                for(int i = 0; i < pixel_size; i++)
                {
                    lable_ param;
                    param.r = tmp_clu_points[i].r;
                    param.b = tmp_clu_points[i].b;
                    param.g = tmp_clu_points[i].g;
                    tmp_clu2_points.push_back(param);
                    // printf("r = %d, b = %d, g = %d \n", tmp_clu_points[i].r, tmp_clu_points[i].b, tmp_clu_points[i].g);
                }
                if(stop_flag == true)
                {
                    break;
                }

                //4. 군집들 중에 가장 가까운 src_points들의 평균을 구한다.
                for(int j = 0; j < k; j++)
                {
                    tmp_r = 0, tmp_g = 0, tmp_b = 0;
                    for(int i = 0; i < pixel_size; i++)
                    {
                        if(j == tmp_clu_points[i].lable1) 
                        {
                            tmp_r += tmp_clu_points[i].r;
                            tmp_g += tmp_clu_points[i].g;
                            tmp_b += tmp_clu_points[i].b;   
                        }
                    }
                    tmp_r /= pixel_size;
                    tmp_g /= pixel_size;
                    tmp_b /= pixel_size;  
                    //printf("tmp_r = %d, tmp_g = %d, tmp_b = %d \n", tmp_r, tmp_g, tmp_b);

                    clu_points[j].x = tmp_r;
                    clu_points[j].y = tmp_g;
                    clu_points[j].z = tmp_b;                    
                    // printf("k = [%d] mean_r = %d, mean_g = %d, mean_b = %d\n", j, tmp_r, tmp_g, tmp_b);
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
                    int r_ = src.at<cv::Vec3b>(j,i)[0];
                    int g_ = src.at<cv::Vec3b>(j,i)[1];
                    int b_ = src.at<cv::Vec3b>(j,i)[2];

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