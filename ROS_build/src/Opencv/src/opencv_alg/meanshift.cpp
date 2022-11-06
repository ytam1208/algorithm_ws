#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


// 아래 줄을 주석처리하면 비디오 영상에 대해 동작 
//#define WEBCAM 



bool mouse_is_pressing = false;
int start_x, start_y, end_x, end_y;
int step = 0;
Mat img_color;
Rect roi;


void swap(int* v1, int* v2) {
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	Mat img_result = img_color.clone();


	if (event == EVENT_LBUTTONDOWN) {
		step = 1;

		mouse_is_pressing = true;
		start_x = x;
		start_y = y;



	}
	else if (event == EVENT_MOUSEMOVE) {

		if (mouse_is_pressing) {

			end_x = x;
			end_y = y;

			step = 2;
		}

	}
	else if (event == EVENT_LBUTTONUP) {

		mouse_is_pressing = false;

		end_x = x;
		end_y = y;

		step = 3;
	}
}


int main()
{
	Mat img_hsv, img_mask, img_ROI;
	Mat objectHistogram;


	int channels[] = { 0 };
	int hsize[] = { 64};
	float range1[] = { 0, 180 };
	const float* histRange[] = { range1 };


#ifdef WEBCAM 
	VideoCapture cap(0);
#else
	VideoCapture cap("../data/vtest.avi");
#endif

	if (!cap.isOpened()) {
		cerr << "video 에러 - 카메라 또는 영상을 열 수 없습니다.\n";
		return -1;
	}

	namedWindow("Color", 1);
	setMouseCallback("Color", mouse_callback);

#ifndef WEBCAM
	Mat img_sceen;
	cap.read(img_sceen);
#endif


	while (1)
	{

#ifdef WEBCAM
		cap.read(img_color);
#else
		if (step == 4)
			cap.read(img_color);
		else
			img_sceen.copyTo(img_color);
#endif

		if (img_color.empty()) {
			cerr << "빈 영상이 캡쳐되었습니다.\n";
			break;
		}

		switch (step)
		{

		case 1:
			circle(img_color, Point(start_x, start_y), 10, Scalar(0, 255, 0), -1);

			break;

		case 2:
			rectangle(img_color, Point(start_x, start_y), Point(end_x, end_y), Scalar(0, 255, 0), 3);

			break;

		case 3:

			if (start_x > end_x) {
				swap(&start_x, &end_x);
				swap(&start_y, &end_y);
			}

			roi = Rect(start_x, start_y, end_x - start_x, end_y - start_y);
			cvtColor(img_color, img_hsv, COLOR_BGR2HSV);
			img_ROI = Mat(img_hsv, roi);

			inRange(img_ROI, Scalar(0., 60., 60.), Scalar(180., 255., 255.), img_mask);

			imshow("ROI", img_ROI);

			calcHist(&img_ROI, 1, channels, img_mask, objectHistogram, 1, hsize, histRange);
			normalize(objectHistogram, objectHistogram, 0, 255, NORM_MINMAX);

			step++;

			break;

		case 4:

			Mat bp;
			cvtColor(img_color, img_hsv, COLOR_BGR2HSV);
			calcBackProject(&img_hsv, 1, channels, objectHistogram, bp, histRange);

			// Tracking
			meanShift(bp, roi, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

			rectangle(img_color, roi, Scalar(0, 0, 255), 2);
			break;

		}


		//if (step < 4)
		cout << step << endl;

		imshow("Color", img_color);


		if (waitKey(25) >= 0)
			break;
	}


	return 0;
}