#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <ctime>

// struct Rand_val
// {
//     std::vector<double> data;
    
//     Rand_val(int number)
//     {
//         for(int i=0; i<number; i++)
//         {
//             double val = rand() % 400;

//             bool found = false;
//             for(int j=0; j<i; j++)
//                 if(data[j] == val)
//                 {
//                     found = true;
//                     break;
//                 }

//             if(found) continue;

//             data.push_back(val);
//         }
//     }
// };
class RANSAC
{
public:
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    cv::Mat srcImage;

    cv::Mat pointx;
    cv::Mat pointy;
    cv::Mat pointx_inv;
    cv::Mat pointx_a;
    cv::Mat LSMX;

    std::vector<int> temp; //모든 에러 데이터를 받을 컨테이너
    std::vector<float> good_line; 
    std::vector<float> good_line_slope; 
    std::vector<float> good_line_ylc;

    std::vector<float> good_line_temp; 


     struct param
    {
        double ran_pointx1; //임의의 첫번째 좌표x1
        double ran_pointy1; //임의의 첫번째 좌표y1

        double ran_pointx2; //임의의 두번째 좌표x2
        double ran_pointy2; //임의의 두번째 좌표y2

        double ran_pointx3; //임의의 첫번째 두번째 좌표를 겹치지 않는 좌표x3
        double ran_pointy3; //임의의 첫번째 두번째 좌표를 겹치지 않는 좌표y3

        double ran_pointx4; //에러율을 가지는 좌표x4
        double ran_pointy4; //에러율을 가지는 좌표y4

        int inlier_point_spot; //첫번째 두번째 좌표에 inlier 한 에러 데이터들의 i 값   ///////vx.inlier_point_spot[i] <= 벡터의 위치

        double distance;  //거리
        double slope = 0; //기울기
        double ylc = 0;   // y절편

        double err_distance = 0; //거리
        double err_slope = 0;
        double err_ylc = 0;

        double LSM_distance; //거리
    };
    
    param index;
    std::vector<param> vx;

    int inliner_th = 5; //인라이어 조건
    int num_inlier = 0;
    float LSM_SUM = 0;
    int tap = 0;

    RANSAC()
    {
        initValue(this);
        make_test();
        count_inlier();
        LSM();
        for_ransac();
    }


    ~RANSAC()
    {
    }


    void initValue(RANSAC *ransac)
    {
        std::srand(std::time(0));
        ransac->srcImage.create(400, 400, CV_8UC3);
    }






    void make_test()
    {
        index.ran_pointx1 = rand() % 400;
        index.ran_pointy1 = rand() % 400;

        //ran_point2가 ran_point1과 동일한 값을 가지지 않도록 do while
        do
        { 
            index.ran_pointx2 = rand() % 400;
        } while (index.ran_pointx1 == index.ran_pointx2);

        do
        {
            index.ran_pointy2 = rand() % 400;
        } while (index.ran_pointy1 == index.ran_pointy2);

        line(srcImage, cv::Point(index.ran_pointx1, index.ran_pointy1), cv::Point(index.ran_pointx2, index.ran_pointy2), cv::Scalar(255, 255, 255));
        //cv::namedWindow("srcImage", cv::WINDOW_AUTOSIZE);
        imshow("testbed_line_image", srcImage);
        cv::waitKey(0);

        index.slope = ((index.ran_pointy2 - index.ran_pointy1) / (index.ran_pointx2 - index.ran_pointx1));
        index.ylc = index.ran_pointy1 - (index.ran_pointx1 * index.slope);


        for (int i = 0; i < 300; i++)
        {
            do
            {
                index.ran_pointx3 = rand() % 400;
                //index.ran_pointy3 = rand() % 400;
            } while ((index.ran_pointx1 == index.ran_pointx3 && index.ran_pointy1 == index.ran_pointy3) ||
                     (index.ran_pointx2 == index.ran_pointx3 && index.ran_pointy2 == index.ran_pointy3));
            //임의의 좌표3이 임의의 좌표1 혹은 좌표2와 같을 경우 다시 좌표3의 랜덤(x,y) 값을 추출한다.

            index.ran_pointx4 = (index.ran_pointx3);
            index.ran_pointy4 = (index.slope * index.ran_pointx3) + index.ylc;
            // std::cout  << "ran_pointx3 = " << index.ran_pointx3 << " ran_pointy3 = " << index.ran_pointy3 << " ran_pointx4 = " << index.ran_pointx4 << " ran_pointy4 = "
            // << index.ran_pointy4 << std::endl;
            vx.push_back(index);
            //index라는 이름을 가진 param 구조체에 벡터 타입으로 0,1번째 컨테이너에 들어간다.      vx[0] = x, vx[0] = y
            //(좌표1, 좌표2)직선의 기울기를 좌표 3(x)에 곱하여 직선에 존재하는 좌표를 생상한 뒤

            int value = 0;
            value = rand() % 5;
            int error = rand() % 50;
            // printf("random value[%d] = %d\n", i, value);

            if (value == 0)
                vx[i].ran_pointx4 = (vx[i].ran_pointx4) + error;
            //vx[vx.size()-1].ran_pointx1 = 4;
            //vx.back().ran_pointx4 = index.ran_pointx4 + 5;

            else if (value == 1)
                vx[i].ran_pointx4 = (vx[i].ran_pointx4) - error;

            else if (value == 2)
                vx[i].ran_pointy4 = (vx[i].ran_pointy4) + error;

            else if (value == 3)
                vx[i].ran_pointy4 = (vx[i].ran_pointy4) - error;

            // else
            //     vx[0].ran_pointx4 =(vx[0].ran_pointx4) + 5;
            //     vx[1].ran_pointy4 = (vx[1].ran_pointy4) + 5;
            line(srcImage, cv::Point(vx[i].ran_pointx4, vx[i].ran_pointy4), cv::Point(vx[i].ran_pointx4, vx[i].ran_pointy4), cv::Scalar(255, 255, 255));
        }

    }


    void count_inlier()
    {
        int value1 = 0;
        value1 = rand() % 300;
        int value2 = 0;
        value2 = rand() % 300;

        double ran_x1 = vx[value1].ran_pointx4; //에러 데이터 중에 있는 좌표 x1
        double ran_y1 = vx[value1].ran_pointy4; //에러 데이터 중에 있는 좌표 y1

        // printf("vx[%d].ran_pointx4 = %lf\n", value1, ran_x1);
        // printf("vx[%d].ran_pointy4 = %lf\n", value1, ran_y1);

        double ran_x2 = vx[value2].ran_pointx4; //에러 데이터 중에 있는 좌표 x2
        double ran_y2 = vx[value2].ran_pointy4; //에러 데이터 중에 있는 좌표 y2

        // printf("vx[%d].ran_pointx4 = %lf\n", value2, ran_x2);
        // printf("vx[%d].ran_pointy4 = %lf\n", value2, ran_y2);

        line(srcImage, cv::Point(ran_x1, ran_y1), cv::Point(ran_x2, ran_y2), cv::Scalar(255, 0, 0));
        //cv::namedWindow("srcImage", cv::WINDOW_AUTOSIZE);
        imshow("errData_line_image", srcImage); 
        cv::waitKey(0);
        // line(srcImage, cv::Point(ran_x1, ran_y1), cv::Point(ran_x2, ran_y2), cv::Scalar(0, 0, 0));

       // printf("inliner_th = %d\n", inliner_th); //몇개의 점들이 인라이어 했는지 알 수 있다.

        index.err_slope = ((ran_y2 - ran_y1) / (ran_x2 - ran_x1)); //err 두 점의 기울기
        index.err_ylc = ran_y1 - (ran_x1 * index.err_slope);       //err 두 점의 y절편

        for (int i = 0; i < 300; i++)
        {
            vx[i].err_distance = abs((index.err_slope * vx[i].ran_pointx4) - vx[i].ran_pointy4 + index.err_ylc) / sqrt((index.err_slope * index.err_slope) + 1);
            //생성된 모든 에러 데이터들과 임의의 두개의 에러 좌표 값에서의 직선과의 거리를 저장한다.        |ax + by + c|
            //y = ax + b            => 점과 직선 사이의 거리 공식      sqrt(a*a + b*b)

            //printf("vx[%d].err_distance = %lf\n", i, vx[i].err_distance); //몇개의 점들이 인라이어 했는지 알 수 있다.

            if (vx[i].err_distance < inliner_th)
            {

                num_inlier++;
                temp.push_back(i);

                //에러의 점 위치와 기존에 생성한 직선과의 거리가 임계값안에 들어오면 vx[count].inlier_point_spot 값에 좌표의 벡터 위치를 알려준다.
            }
        }
        //printf("count_inlier_point_spot = %d\n", num_inlier); //몇개의 점들이 인라이어 했는지 알 수 있다.
                                                              //printf("ran_pointx412: %lf\n", vx[temp[0]].ran_pointx4);
    }

    void LSM()
    {
    back:
        for (int i = 0; i < num_inlier; i++)
        {
            //double datax[2] = {vx[vx[i].inlier_point_spot].ran_pointx4, 1};
            float datax[2] = {(float)vx[temp[i]].ran_pointx4, 1};

            //printf("%lf %lf \n", datax[0], datax[1]);

            cv::Mat rowx(1, 2, CV_32FC1, datax);
            pointx.push_back(rowx); //pointx 라는 매트릭스에 rowx 값을 넣어준다.    //이 모든 값은 인라이어에 들어와 있는 에러 데이터들의 좌표

            cv::Mat rowy(1, 1, CV_32FC1, (float)vx[temp[i]].ran_pointy4);
            pointy.push_back(rowy); //pointy 라는 매트릭스에 rowy 값을 넣어준다.
        }

        pointx_inv = (pointx.t() * pointx).inv();
        pointx_a = pointx_inv * pointx.t();
        LSMX = pointx_a * pointy;

        // std::cout << "pointx_inv.size() = " << pointx_inv.size() << std::endl;
        // std::cout << "pointx_a.size() = " << pointx_a.size() << std::endl;
        // std::cout << "LSM slope = " << LSMX.at<float>(0, 0) << " LSM ylc = " << LSMX.at<float>(0, 1) << std::endl;

        line(srcImage, cv::Point(0, LSMX.at<float>(0, 1)), cv::Point(srcImage.cols, (srcImage.cols * LSMX.at<float>(0, 0)) + LSMX.at<float>(0, 1)), cv::Scalar(0, 255, 0));
        cv::namedWindow("srcImage", cv::WINDOW_AUTOSIZE);
        imshow("LSM_line_image", srcImage);
        cv::waitKey(0);
        // line(srcImage, cv::Point(0, LSMX.at<float>(0, 1)), cv::Point(srcImage.cols, (srcImage.cols * LSMX.at<float>(0, 0)) + LSMX.at<float>(0, 1)), cv::Scalar(0, 0, 0));

        for (int i = 0; i < 300; i++)
        {

            double ran_x1 = vx[i].ran_pointx4; //에러 데이터 중에 있는 좌표 x1
            double ran_y1 = vx[i].ran_pointy4; //에러 데이터 중에 있는 좌표 y1
            index.err_distance = abs((LSMX.at<float>(0, 0) * ran_x1) - ran_y1 + LSMX.at<float>(0, 1)) / sqrt((LSMX.at<float>(0, 0) * LSMX.at<float>(0, 0)) + 1);

            LSM_SUM = LSM_SUM + index.err_distance;
        }

        //printf("LSM_SUM = %lf \n", LSM_SUM);
    }

    void for_ransac()
    {
        good_line_temp.push_back(LSM_SUM);

        for (int i = 0; i < 17 ; i++)
        {

            count_inlier();
            LSM();

            //std::cout << "good_line_temp = " << good_line_temp.back() << std::endl;
            good_line.push_back(LSM_SUM);
            //std::cout << "LSM_SUM = " << LSM_SUM << std::endl;

            if (good_line[i] < good_line_temp.back())
            {
                std::cout << "cheer up!!" << std::endl;
                good_line_temp.push_back(good_line.back());

                good_line_slope.push_back((LSMX.at<float>(0, 0))); //기울기
                good_line_ylc.push_back(LSMX.at<float>(0, 1));     //y절편
                
            }
        }

        // std::cout << "good_line_temp = " << good_line_temp.back() << " good_line_slope = " << good_line_slope.back() << " good_line_ylc = " << good_line_ylc.back() << std::endl;

        // line(srcImage, cv::Point(0, good_line_ylc.back()), cv::Point(srcImage.cols, (good_line_slope.back() * srcImage.cols) + LSMX.at<float>(0, 1)), cv::Scalar(0, 0, 255));
        // //cv::namedWindow("srcImage", cv::WINDOW_AUTOSIZE);
        // imshow("good_line_image", srcImage);
        // cv::waitKey(0);
    }
};

int main()
{
    RANSAC ransac = RANSAC();

    return 0;
}

