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
            cv::Point3f points;
            int r, g, b;
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

        cv::Mat src, dst;
        std::vector<cv::Point3f> src_points;           //이미지에서의 RGB 컨테이너
        std::vector<cv::Point3f> clu_points;           //임의의 군집 좌표
        K_means()
        {
            initvalue(this);
            MSG();
            IGG();
            runloop();
            //make_kmeans();

        }
        ~K_means()
        {

        }
        void make_kmeans()
        {
            for(int j = 0; j < k; j++)
            {
                for(int i = 0; i < pixel_size; i++)
                {
                    if(j == tmp_clu_points[i].lable1)
                    {
                        src_points[i].x = tmp_clu_points[i].r;
                        src_points[i].y = tmp_clu_points[i].g;
                        src_points[i].z = tmp_clu_points[i].b;
                        // printf("tmp_clu_points.r = %d, tmp_clu_points.g = %d, tmp_clu_points.b = %d\n", tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b);
                        // printf("src_point.x = %d, src_point.y = %d, src_point.z = %d\n", src_points[i].x, src_points[i].y, src_points[i].z);
                    }
                }
            }
                // for(int j = 0; j < width; j++)
                //     for(int i = 0; i < height; i++)
                //     {
                //         int r_, g_, b_;
                //         r_ = 0, g_ = 0; b_ = 0;
                        
                //         r_ = src_points[z].x;
                //         g_ = src_points[z].y;
                //         b_ = src_points[z].z;
                //         // printf("r = %d, g = %d, b = %d \n", r_, g_, b_);

                //         // dst.at<cv::Vec3b>(j,i)[0] = r_;
                //         // dst.at<cv::Vec3b>(j,i)[1] = g_;
                //         // dst.at<cv::Vec3b>(j,i)[2] = b_;
                        
                //     }

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
                        //printf("distance = %d\n", distance);
                        if(distance < min_dist)
                        {
                            min_dist = distance;
                            complete_k = i;
                            check_distane = true;
                        }
                    }
                    //printf("j = %d, pixel_size = %d\n", j, pixel_size);
                    if(check_distane == true)  //3. 군집과 가장 가까운 src_points, 몇 번째 군집인지를 저장한다.
                    {
                        tmp_clu_points[j].lable1 = complete_k;
                        // printf("tmp_clu_points[%d].lable1 = %d\n", j, tmp_clu_points[j].lable1);
                        //printf("r = %d, g = %d, b = %d \n", r, g, b);
                        // tmp_clu_points[j].points.x = r;
                        // tmp_clu_points[j].points.y = g;
                        // tmp_clu_points[j].points.z = b;
                        tmp_clu_points[j].r = r;
                        tmp_clu_points[j].g = g;
                        tmp_clu_points[j].b = b;
                    }
                }
                if(first_check == false)
                {
                    break;
                }
                else if(first_check == true)
                {
                    first_check = false;
                }

                mean_value.push_back(mean);
                for(int j = 0; j < k; j++)
                {
                    int r = 0, g = 0, b = 0;
                    for(int i = 1; i < pixel_size; i++)
                    {
                        if(j == tmp_clu_points[i].lable1) //4. 군집과 군집 당 가장 가까운 src_point들의 평균을 구한다.
                        {
                            r += tmp_clu_points[i].r;
                            g += tmp_clu_points[i].g;
                            b += tmp_clu_points[i].b;                            
                        }
                    }
                    if(r == 0)
                        r = 0;
                    else if(g == 0)
                        g = 0;
                    else if(b == 0)
                        b = 0; 
                    else{
                        r /= pixel_size;
                        g /= pixel_size;
                        b /= pixel_size;
                    }
                    mean_value[j].mean_r = r;
                    mean_value[j].mean_g = g;
                    mean_value[j].mean_b = b;
                }



                
                for(int j = 0; j < pixel_size; j++)
                {
                    tmp_clu_points[j].r = mean_value[j].mean_r;
                    tmp_clu_points[j].g = mean_value[j].mean_g;
                    tmp_clu_points[j].b = mean_value[j].mean_b;                            
                    printf("tmp_clu_point. r = %d\n", tmp_clu_points[j].r);
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