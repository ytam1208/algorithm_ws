nclude <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// double verticality_sobelmask [5][5]= {{ -1,  -1,  0,  1,  1},
//                                       { -1,  -1,  0,  1,  1},
//                                       { -2,  -2 , 0,  2,  2},
//                                       { -1,  -1,  0,  1,  1},
//                                       { -1,  -1,  0,  1,  1}}; //소벨 마스크(수직 마스크)

// double horizontality_sobelmask [5][5]= {{ 1,  1,  2,  1,  1},
//                                         { 1,  1,  2,  1,  1},
//                                         { 0,  0,  0,  0,  0},
//                                         {-1, -1, -2, -1, -1},
//                                         {-1, -1, -2, -1, -1}}; //소벨 마스크(수평 마스크)

// double verticality_sobelmask [3][3]= {{-1,  0,  1},                                     
//                                       {-2 , 0,  2},                                     
//                                       {-1,  0,  1}}; //소벨 마스크(수직 마스크)

// double horizontality_sobelmask [3][3]= {{1,  2,  1},
//                                         {0,  0,  0},
//                                         {-1, -2, -1}}; //소벨 마스크(수평 마스크)

cv::Mat maskX = (cv::Mat_<double>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);

cv::Mat maskY = (cv::Mat_<double>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
          
cv::Mat GaussianFilter(cv::InputArray _src, cv::OutputArray _dst, float thresh); 
//가우시안 필터링
cv::Mat sobelFilter(cv::InputArray _src , cv::OutputArray _dst, float thresh); 
//결과 영상에 소벨연산자 적용하여 에지 강도와 에지 방향 맵을 구한다.
cv::Mat non_max_suppression(cv::InputArray _src, cv::InputArray _src2, cv::InputArray Input_src, cv::OutputArray _dst, float thresh);
//비최대 억제
cv::Mat follow_edge(cv::InputArray _src, int y, int x, float thresh);
//에지를 추적하는 재귀 함수
cv::Mat for_follow_edge(cv::InputArray _src, int y, int x, float thresh);

cv::Mat non_max_Image;
cv::Mat angle;
cv::Mat power;
cv::Mat cannyImage;

double PI = 3.14159265;
int Thigh = 120;
int Tlow = 40;
int visited[999][999];

int main(){
    
    cv::Mat srcImage = cv::imread("123.jpg", cv::IMREAD_GRAYSCALE);
    if (srcImage.empty())
        return 1;
    
    cv::Mat dstImage;
    cv::Mat sobelImage;

    // imshow("Input_Image", srcImage);
    // cv::waitKey(); 

    dstImage = GaussianFilter(srcImage, dstImage, 256); //가우시안 필터 적용
    // imshow("GaussianFilter_Image", dstImage);
    // cv::waitKey();

    sobelImage = sobelFilter(dstImage,sobelImage, 256);
//     imshow("output_Image", sobelImage);  
//     cv::waitKey();

     imshow("cannyImage", sobelImage);  
     cv::waitKey();
    return 0;
}


cv::Mat sobelFilter(cv::InputArray _src, cv::OutputArray _dst, float thresh)
{
    cv::Mat src = _src.getMat();
    cv::Mat power = _src.getMat();//그래디언트 크기를 저장하는 Mat
    cv::Mat angle = _src.getMat(); //그래디언트 방향을 저장하는 Mat
    cv::Mat non_max = _src.getMat();;
    cv::Mat test = _dst.getMat();;
    // _dst.create(src.size(), src.type());
    //cv::Mat _dst;
    // dst.setTo(cv::Scalar::all(0));

    int height = src.rows;
    int width = src.cols;
    double value = 0;

    // int ver_value = 0;
    // int hor_value = 0;


    double sumEdgeX;
    double sumEdgeY;
    

    for (int yimage = 1; yimage < height - 1; ++yimage){
        

        for (int ximage = 1; ximage < width - 1; ++ximage){
                        
            sumEdgeX = 0;
            sumEdgeY = 0;
            int div = 0;

            for (int xmask = 0; xmask < 3; xmask++)
                {
                    sumEdgeX += src.at<uchar>(xmask, 2) * maskX.at<uchar>(xmask, 2);
                    sumEdgeX += src.at<uchar>(xmask, 0) * maskX.at<uchar>(xmask, 0);
                }
            for (int ymask = 0; ymask < 3; ymask++)
                {
                    sumEdgeY += src.at<uchar>(2, ymask) * maskY.at<uchar>(2, ymask);
                    sumEdgeY += src.at<uchar>(0, ymask) * maskY.at<uchar>(0, ymask);
                }

            
            value = sqrt(pow(sumEdgeY, 2) + pow(sumEdgeX, 2));  //에지 강도 //threshold 적용


            if (sumEdgeX == 0 && sumEdgeY < 0)
                {
                    div = -90;
                }
            else if (sumEdgeX == 0 && sumEdgeY > 0)
                {
                    div = 90;
                }
            else if (sumEdgeX == 0 && sumEdgeY == 0)
                {
                    div = 0;
                }
            else
                {
                    div = (atan(sumEdgeY / sumEdgeX) * 180 / PI) + 90;
                }

            if ((div > 0 && div <= 22.5) || (div > 157.5 && div <= 202.5))
                    div = 0;

            else if (div > 22.5 && div <= 67.5 || (div > 202.5 && div <= 247.5))
                    div = 1;

            else if (div > 67.5 && div <= 112.5 || (div > 247.5 && div <= 292.5))
                    div = 2;

            else if (div > 112.5 && div <= 157.5 || (div > 292.5 && div <= 337.5))
                    div = 3;

            else
                    div = 0;

            power.at<uchar>(yimage, ximage) = value; //그래디언트 세기
            angle.at<uchar>(yimage, ximage) = div; //엣지 방향

            non_max = non_max_suppression(power, angle, src, non_max, 256);


            for_follow_edge(non_max, yimage, ximage, 256);
            test.at<uchar>(yimage, ximage) = follow_edge(non_max, yimage, ximage, 256);

        }

    }
    return test;    
}



cv::Mat for_follow_edge(cv::InputArray _src, int y,int x, float thresh)
{
    cv::Mat src = _src.getMat();
//    cv::Mat dst = _src.getMat();    
    int height = src.rows;
    int width = src.cols;


    for(int j =1; j < height-1; j++){
        for(int i=1; i < width-1; i++)
        {
              if(src.at<uchar>(j,i) > Thigh && visited[j][i] == 0) follow_edge(src,j,i,256);
        }
    }
        

}



cv::Mat follow_edge(cv::InputArray _src, int y,int x, float thresh)
{
    visited[y][x] = 1;
    cv::Mat src = _src.getMat();
    
    cannyImage.at<uchar>(y,x) = 255;

    for (int j = y-1; j < y+2; j++)
        for (int i = x-1; i < x+2; i++)
        {
            if (src.at<uchar>(j, i) > Tlow && visited[j][i] == 0)
                follow_edge(src, j, i, 256);
        }

    return cannyImage;
}





cv::Mat GaussianFilter(cv::InputArray _src, cv::OutputArray _dst, float thresh)
{
    
    float g = thresh;
    float a = 64 / g;
    float b = 32 / g;
    float c = 16 / g;
    float G[3][3] = { c, b, c,
                      b, a, b,
                      c, b, c };

    cv::Mat src = _src.getMat();

    _dst.create(src.size(), src.type());
    cv::Mat dst = _dst.getMat();
    dst.setTo(cv::Scalar::all(0));  //setTo 행렬 내 모든 스칼라 값을 0으로 초기화.


    for(int i = 2; i < src.rows -2; i++)
    {
        for(int j = 2; j < src.cols -2; j++)
        {
            for(int k = 0; k <= 2; k++)
            {
                for(int l = 0; l <= 2; l++)
                {
                    dst.at<uchar>(i, j) += src.at<uchar>(i-2+k, j-2+l) * G[k][l];
                }
            }
        }
    }

    return dst;
}


cv::Mat non_max_suppression(cv::InputArray _src, cv::InputArray _src2, cv::InputArray Input_src, cv::OutputArray _dst, float thresh)
{
    cv::Mat power_image = _src.getMat();
    cv::Mat angle_image = _src2.getMat();

    cv::Mat origin_image = Input_src.getMat();

    int height = origin_image.rows;
    int width = origin_image.cols;

    for(int y = 1; y < height -1; y++)
    {
        for(int x = 1; x < width -1; x++)
        {
            if(angle_image.at<uchar>(y,x) == 0)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                       power_image.at<uchar>(y, x) = 0;
                    };
            }

            else if(angle_image.at<uchar>(y,x) == 1)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                        power_image.at<uchar>(y, x) = 0;
                    };
            }

            else if(angle_image.at<uchar>(y,x) == 2)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                        power_image.at<uchar>(y, x) = 0;
                    };
            }

            else if(angle_image.at<uchar>(y,x) == 3)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                        power_image.at<uchar>(y, x) = 0;
                    };
            }
            else if(angle_image.at<uchar>(y,x) == 4)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                        power_image.at<uchar>(y, x) = 0;
                    };
            } 
            else if(angle_image.at<uchar>(y,x) == 6)
            {
                if( power_image.at<uchar>(y, x) <= power_image.at<uchar>(y - 1, x) ||
                    power_image.at<uchar>(y, x) <= power_image.at<uchar>(y + 1, x) )
                    {
                        power_image.at<uchar>(y, x) = 0;
                    };
            }            
        }
    }

    return power_image;
}



