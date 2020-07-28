#include "Opencv/OpencvBase.hpp"

//Color to Gray ptr 
//Mat::ptr
//포인터를 사용하는 방법으로  at 방법에 비해 빠른 속도를 보여줍니다.  현재 row의 포인터를 지정한 타입으로 미리 계산해서 가져오기 때문입니다.
//OpenCV의 Mat 구조상 row의 포인터를 한번 가져오고 이후 사용하면 픽셀 위치 계산이 빠르게 됩니다. 

int main(int argc, char ** argv)
{
 //   printf("hi_lena :)\n");
    opencv_img img_;
    
    int height = img_.src.rows;
    int weight = img_.src.cols;

    cv::Mat input_img = img_.src;
    cv::Mat gray_img(height, weight, CV_8UC1);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < weight; j++)
        {
            uchar* pointer_input = input_img.ptr<uchar>(j);
            uchar* pointer_output = gray_img.ptr<uchar>(j);

            for(int z = 0; z < weight; z++)
            {
                uchar b = pointer_input[z * 3 + 0];
                uchar g = pointer_input[z * 3 + 1];
                uchar r = pointer_input[z * 3 + 2];

                pointer_output[z] = (b + g + r) / 3.0;
            }
        }
    cv::imshow("color_img", input_img);
    cv::imshow("gray_img", gray_img);
    cv::waitKey(0);

    return 0;
}
