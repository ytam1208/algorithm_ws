#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
//https://ottuging.tistory.com/15
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
        std::vector<lable_> tmp_clu2_points;

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
            while(1)
            {
                for(int j = 0; j < width; j++)
                    for(int i = 0; i < height; i++)
                    {
                        if(k_ == k)
                            k_ = 0;
                        if(point_size > pixel_size)
                        {
                            goto EXIT;
                        }
                        if(k_ == tmp_clu_points[point_size].lable1)
                        {
                            // printf("go\n");
                            int r1 = tmp_clu2_points[point_size].r;
                            int g1 = tmp_clu2_points[point_size].b;
                            int b1 = tmp_clu2_points[point_size].g;
                            dst.at<cv::Vec3b>(j,i)[0] = r1;
                            dst.at<cv::Vec3b>(j,i)[1] = g1;
                            dst.at<cv::Vec3b>(j,i)[2] = b1;
                            printf("r1 = %d, b1 = %d, g1 = %d\n", r1, b1, g1);
                        }
                        point_size++;
                        k_++;
                    }
            }
            EXIT:
                cv::imshow("Result_img", dst);
                cv::waitKey(0);
        }
        void runloop() 
        {
            bool first_check = true;
            int distance;

            while(true)
            {
                // printf("pixel_size = %d\n", pixel_size);
                bool check_distane = false;
                for(int j = 0; j < pixel_size; j++)
                {
                    int min_dist = 500000;
                    int complete_k = -1;
                    tmp_clu_points.push_back(param);
                    tmp_clu2_points.push_back(param);
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
                            check_distane = true;
                        }
                    }
                    if(check_distane == true)  //3. 군집과 가장 가까운 src_points, 몇 번째 군집인지를 저장한다.
                    {
                        tmp_clu_points[j].lable1 = complete_k;
                        tmp_clu_points[j].r = r;
                        tmp_clu_points[j].g = g;
                        tmp_clu_points[j].b = b;
                        //printf("r = %d, g = %d, b = %d \n", r, g, b);
                    }
                }

                //4. 군집들 중에 가장 가까운 src_points들의 평균을 구한다.
                mean_value.push_back(mean);
                int tmp_r = 0, tmp_g = 0, tmp_b = 0;
                for(int j = 0; j < k; j++)
                {
                    for(int i = 0; i < pixel_size; i++)
                    {
                        if(j == tmp_clu_points[i].lable1) 
                        {
                            tmp_r += tmp_clu_points[i].r;
                            tmp_g += tmp_clu_points[i].g;
                            tmp_b += tmp_clu_points[i].b;                            
                        }
                    }
                    tmp_r /= (pixel_size);
                    tmp_g /= (pixel_size);
                    tmp_b /= (pixel_size);  

                    mean_value[j].mean_r = tmp_r;
                    mean_value[j].mean_g = tmp_g;
                    mean_value[j].mean_b = tmp_b;
                }
                for(int j = 0; j < k; j++)
                {
                    for(int i = 0; i < pixel_size; i++)
                    {
                        if(j == tmp_clu_points[i].lable1)
                        {
                            tmp_clu2_points[i].r = mean_value[j].mean_r;
                            tmp_clu2_points[i].g = mean_value[j].mean_g;
                            tmp_clu2_points[i].b = mean_value[j].mean_b;
                            // printf("tmp_clu2_point[%d].r = %d\n", i, tmp_clu2_points[i].r);
                            // printf("tmp_clu2_point[%d].r = %d\n", i, tmp_clu2_points[i].g);
                            // printf("tmp_clu2_point[%d].r = %d\n", i, tmp_clu2_points[i].b);
                        }
                    }
                }

                //여기까지가 첫번째 랜덤으로 주어진 군집에서 가까운 RGB들의 평균
                //두번째부터 새로운 중심점들인 tmp_clu2_points와 거리 차이를 계산해준다.
                // int magchage = 0;
                // bool threshold = false;
                //     for(int i = 0; i < pixel_size; i++)
                //     {
                //         magchage = sqrt(
                //         pow(tmp_clu2_points[i].r - tmp_clu_points[i].r, 2) + 
                //         pow(tmp_clu2_points[i].g - tmp_clu_points[i].g, 2) +
                //         pow(tmp_clu2_points[i].b - tmp_clu_points[i].b, 2));
                //         if(magchage > 5)
                //         {   
                //             threshold = true;
                //             break;
                //         }
                //     }
                //중심점들끼리의 거리가 똑같을수가 있다 너무 가까우면 다시 루프를 돌게한다.
                // bool is_same = false;
                // for(int k1 = 0; k1 < k -1; k1++)
                //     for(int k2 = k1+1; k2 < k; k2++)
                //     {
                //         if(sqrt(pow(tmp_clu_points[k1].r - tmp_clu_points[k2].r, 2) + pow(tmp_clu_points[k1].r - tmp_clu_points[k2].r, 2) + 
                //         pow(tmp_clu_points[k1].r - tmp_clu_points[k2].r, 2)) <= 5)
                //         {
                //             k1 = pixel_size;
                //             is_same = true;
                //             break;
                //         }

                //     }
                if(first_check == true)
                {
                    first_check = false;
                    continue;
                }
                else if(first_check == false)//threshold == false)// && is_same == false)
                {
                    //현재 중심점들과의 이전 중심점들과의 거리 차이가 5 이하이면 종료한다.
                    break;
                }
                // else
                // {
                //     for(int i = 0; i < pixel_size; i++)
                //     {
                //         clu_points[i].x = tmp_clu2_points[i].r;
                //         clu_points[i].y = tmp_clu2_points[i].g;
                //         clu_points[i].z = tmp_clu2_points[i].b;
                //     }
                // }
            }
            // for (int k = 0; k < pixel_size; k++)
    		//     std::cout << "Final Ks : " << clu_points[k].x << " " << clu_points[k].y << " " << clu_points[k].z << std::endl;

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