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
    double r, g, b;
    double tmp_r, tmp_g, tmp_b;

    double cr, cg, cb;

    struct lable_
    {
        int lable1;
        double r;
        double g;
        double b;
    };
    lable_ param;
    std::vector<lable_> tmp_clu_points; //개별 군집과 가까운 RGB컨테이너 배열의 위치를 lable 별로 저장

    cv::Mat src;
    cv::Mat dst;

    std::vector<cv::Point3f> src_points; //이미지에서의 RGB 컨테이너
    std::vector<cv::Point3f> clu_points; //임의의 군집 좌표
    std::vector<cv::Point3f> clu2_points;

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

        for (int j = 0; j < width; j++)
            for (int i = 0; i < height; i++)
            {
                double r1 = tmp_clu_points[point_size].r;
                double g1 = tmp_clu_points[point_size].g;
                double b1 = tmp_clu_points[point_size].b;
                
                dst.at<cv::Vec3b>(j, i)[0] = r1;
                dst.at<cv::Vec3b>(j, i)[1] = g1;
                dst.at<cv::Vec3b>(j, i)[2] = b1;
                point_size++;
            }
        cv::imshow("Result_img", dst);

        cv::Mat t_plot(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));
        cv::Mat t_plot2(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));
        cv::Mat t_plot3(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));

            for (int i = 0; i < (int)tmp_clu_points.size(); i++)
            {
//                cv::circle(t_plot, cv::Point2f(tmp_clu_points[i].r, tmp_clu_points[i].g), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
//                cv::circle(t_plot2, cv::Point2f(tmp_clu_points[i].g, tmp_clu_points[i].b), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
//                cv::circle(t_plot3, cv::Point2f(tmp_clu_points[i].r, tmp_clu_points[i].b), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
                cv::circle(t_plot, cv::Point2f(src_points[i].x, src_points[i].y), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
                cv::circle(t_plot2, cv::Point2f(src_points[i].y, src_points[i].z), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
                cv::circle(t_plot3, cv::Point2f(src_points[i].x, src_points[i].z), 3, cv::Scalar(tmp_clu_points[i].r, tmp_clu_points[i].g, tmp_clu_points[i].b));
            }

        cv::imshow("Result_RG", t_plot);
        cv::imshow("Result_GB", t_plot2);
        cv::imshow("Result_RB", t_plot3);
        cv::waitKey(0);
    }
    void runloop()
    {
        double distance;
        bool is_first = true;
        bool stop_flag = false;

        while (true)
        {
            printf("gopgo\n");
            tmp_clu_points.clear();
            for (int j = 0; j < pixel_size; j++)
            {
                int min_dist = -1;
                int complete_k = -1;

                for (int i = 0; i < k; i++)
                {
                    r = src_points[j].x;
                    g = src_points[j].y;
                    b = src_points[j].z;

                    cr = clu_points[i].x;
                    cg = clu_points[i].y;
                    cb = clu_points[i].z;
                    //printf("cr = %d, cg = %d, cb = %d\n", cr, cg, cb);
                    distance = sqrt(pow(r - cr, 2) + pow(g - cg, 2) + pow(b - cb, 2));
                    if (distance < min_dist || min_dist < 0)
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

            if (is_first == false)
            {
                int count = 0;
                for (int i = 0; i < k; i++)
                {
                    if (clu2_points[i].x == clu_points[i].x //이 배정이 직전 배정과 값이 동일하다면 더이상의 변화 X 그러므로 stop flag = true
                        && clu2_points[i].y == clu_points[i].y 
                            && clu2_points[i].z == clu_points[i].z)
                    {
                        count++;
                        if (count == k)
                            stop_flag = true;
                    }
                }
                printf("count = %d\n",count);
            }
            else if (is_first == true)
                is_first = false;

            if (stop_flag == true) // 이 배정이 첫 배정이 아니고 이전 루프의 배정과 같으면 현재 평균 RGB를 넣어준다.
            {
                for (int j = 0; j < k; j++)
                {
                    for (int i = 0; i < pixel_size; i++)
                    {
                        if (tmp_clu_points[i].lable1 == j)
                        {
                            tmp_clu_points[i].r = clu_points[j].x;
                            tmp_clu_points[i].g = clu_points[j].y;
                            tmp_clu_points[i].b = clu_points[j].z;
                        }
                    }
                }
                break;
            }

            clu2_points.clear();
            for(int i = 0; i < k; i++)
            {               
                float clr = clu_points[i].x;
                float clg = clu_points[i].y;
                float clb = clu_points[i].z;
                clu2_points.push_back(cv::Point3f(clr,clg,clb));         
            }


            //4. 군집들 중에 가장 가까운 src_points들의 평균을 구한다.
            for (int j = 0; j < k; j++)
            {
                tmp_r = 0, tmp_g = 0, tmp_b = 0;
                int pixel_size_for_k = 0;
                for (int i = 0; i < pixel_size; i++)
                {
                    if (j == tmp_clu_points[i].lable1)
                    {
                        tmp_r += tmp_clu_points[i].r;
                        tmp_g += tmp_clu_points[i].g;
                        tmp_b += tmp_clu_points[i].b;
                        pixel_size_for_k++;
                    }
                }
                tmp_r /= (double)pixel_size_for_k;
                tmp_g /= (double)pixel_size_for_k;
                tmp_b /= (double)pixel_size_for_k;

                clu_points[j].x = tmp_r;
                clu_points[j].y = tmp_g;
                clu_points[j].z = tmp_b;
            }
        }
    }
    void IGG() //Make Clu group    2.k개의 군집 중심을 초기화한다.(랜덤으로 k개의 좌표를 생성해준다)
    {
        srand((unsigned int)time(NULL));
        std::vector<int> idx_list;
        for(int i=0; i<k; i++)
        {
            while(1)
            {
                int idx = rand() % pixel_size;
                bool found_same = false;
                for(int j=0; j<(int)idx_list.size(); j++)
                    if(idx_list[j] == idx) found_same = true;
                if(!found_same) 
                {
                    idx_list.push_back(idx);
                    break;
                }
            }
            clu_points.push_back(cv::Point3f(src_points[idx_list[i]].x, src_points[idx_list[i]].y, src_points[idx_list[i]].z));
        }
    }

    void MSG() //Make Sample group 1.화소를 X = (R,G,B) 형태로 변환하여 샘플 집합 X 컨테이너로 구성한다.
    {
        for (int j = 0; j < width; j++)
            for (int i = 0; i < height; i++)
            {
                double r_ = (double)src.at<cv::Vec3b>(j, i)[0];
                double g_ = (double)src.at<cv::Vec3b>(j, i)[1];
                double b_ = (double)src.at<cv::Vec3b>(j, i)[2];

                src_points.push_back(cv::Point3f(r_, g_, b_));
            }

        cv::Mat plot(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));
        cv::Mat plot2(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));
        cv::Mat plot3(255, 255, CV_8UC3, cv::Scalar(255, 255, 255));
        for (int i = 0; i < (int)src_points.size(); i++)
        {
            cv::circle(plot,  cv::Point2f(src_points[i].x, src_points[i].y), 3, cv::Scalar(src_points[i].x, src_points[i].y, src_points[i].z));
            cv::circle(plot2, cv::Point2f(src_points[i].y, src_points[i].z), 3, cv::Scalar(src_points[i].x, src_points[i].y, src_points[i].z));
            cv::circle(plot3, cv::Point2f(src_points[i].x, src_points[i].z), 3, cv::Scalar(src_points[i].x, src_points[i].y, src_points[i].z));
        }
        cv::imshow("RG", plot);
        cv::imshow("GB", plot2);
        cv::imshow("RB", plot3);
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