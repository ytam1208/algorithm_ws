nclude <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;    //쓰지말기
using namespace std;   //쓰지말기 

void histgoram();
void histgoram_equalization();

Mat img,img2, img_histogram, img_histogram2;

int main()
{
    histgoram();
    histgoram_equalization();
	return 0;
}


 void histgoram()
 {
    int sum = 0;
    


   	img = imread("img_gray.jpg", IMREAD_COLOR);
	if (img.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		exit(1);
	}

	Mat img_gray;

	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	//imwrite("gray image.jpg", img_gray); //이미지 파일 저장
    
    //입력 그레이스케일 영상의 히스토그램 계산
    int histogram[256] = { 0, };

    for (int y = 0; y < img.rows; y++){

        for (int x = 0; x < img.cols; x++){
            int value = img_gray.at<uchar>(y, x);
            histogram[value] += 1;
        }
    }

     //히스토그램 그리기
    img_histogram = Mat( 300, 300, CV_8UC1, Scalar(255)); //흰색으로 초기화된 300x300 생성(그래프 용)

    int max = -1;

    for (int i = 0; i < 256; i++)
        if (max < histogram[i]){ 
            max = histogram[i];
        }

    for (int i = 0; i < 256; i++){
        int histo = 300 * histogram[i] / (float)max;
        line(img_histogram, Point(i + 10, 300), Point(i + 10, 300 - histo), Scalar(0,0,0));
        //그래프 용 300 흰색 바탕에 검은색 라인 생성 10 간격으로.....최대 값 300 
    }

	imshow("Grayscale", img_gray);               //그레이 그림 
    imshow("img_histogram_line.jpg", img_histogram); //그레이 그림 그래프

//    cv::waitKey(0);

 }


void histgoram_equalization()
{
    int sum = 0;
    int histgoram[256] ={ 0, };
    int histgoram2[256] ={ 0, };
    int sum_hist[256] = { 0, };
   	
       
    img2 = imread("img_gray.jpg", IMREAD_COLOR);
	
    if (img2.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		exit(1);
	}

	Mat img_gray( img2.rows, img2.cols, CV_8UC1, Scalar(255));
    Mat img_gray2( img2.rows, img2.cols, CV_8UC1, Scalar(255));
    
    


	cvtColor(img2, img_gray, COLOR_BGR2GRAY);

    for (int y = 0; y < img2.rows; y++){ // 히스토그램 계산

        for (int x = 0; x < img2.cols; x++){
            int value = img_gray.at<uchar>(y, x);
            histgoram[value] += 1;
        }
    }


    double scale_factor = 255.0/((float)(img2.cols * img2.rows));

    for(int i = 0; i<256; i++) // 정규화된 합을 계산 누적 히스토그램 
    {
        sum += histgoram[i];
        sum_hist[i] = sum;

    }

    for(int i = 0; i<256; i++){   // 정규화
        
        sum_hist[i] =round(sum_hist[i]*scale_factor);
    }

     for (int y = 0; y < img2.rows; y++){ // 히스토그램 계산

        for (int x = 0; x < img2.cols; x++){
             int value = img_gray.at<uchar>(y, x);
             img_gray2.at<uchar>(y, x) = sum_hist[value];

             histgoram2[sum_hist[value]]++;
        }
    }

    img_histogram2 = Mat( 255, 255, CV_8UC1, Scalar(255)); //흰색으로 초기화된 300x300 생성(그래프 용)

    double max = -1;

    for (int i = 0; i < 256; i++)
        if (max < histgoram2[i]){ 
            max = histgoram2[i];
        }

    for (int i = 0; i < 256; i++){
        
        double histo = 255 * histgoram2[i] / (double)max;
        line(img_histogram2, Point(i + 10, 255), Point(i + 10, 255 - histo), Scalar(0,0,0));
        //그래프 용 255 흰색 바탕에 검은색 라인 생성 10 간격으로.....최대 값 255 
    }

    imshow("img_histogram_eq.jpg", img_gray2);
    imshow("img_histogram_line_eq_line.jpg", img_histogram2);
    cv::waitKey(0);


}

    




