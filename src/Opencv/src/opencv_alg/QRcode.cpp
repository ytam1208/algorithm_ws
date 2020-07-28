#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(void)
{
	VideoCapture cap(0);

	if (!cap.isOpened()) {
		cerr <<"Camera open failed!" << endl;
		return -1;
	}

	QRCodeDetector detector;

	Mat frame, gray;
	while (1) {
		cap >> frame;

		if (frame.empty()) {
			cerr << "Frame load failed!" << endl;
			return -1;
		}

		cvtColor(frame, gray, COLOR_BGR2GRAY);

		vector<Point> points;
		if (detector.detect(gray, points)) {
			polylines(frame, points, true, Scalar(0, 255, 255), 2);

			String info = detector.decode(gray, points);
			if (!info.empty()) {
				polylines(frame, points, true, Scalar(0, 0, 255), 2);
				cout << info << endl;
			}
		}

        // Mat dst;
        // resize( frame, dst, Size(256,256), 0, 0, CV_INTER_NN);
        namedWindow("frame", WINDOW_NORMAL);
        resizeWindow("frame", 1280, 720);

		imshow("frame", frame);
		if (waitKey(1) == 27)
			break;
	}

	return 0;
}

