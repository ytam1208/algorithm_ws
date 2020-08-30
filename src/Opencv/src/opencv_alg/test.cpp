#include <iostream>
#include <vector>
#include <ctime>

#include "opencv2/opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

#define MAX_DIST 50000

cv::Mat KMeans_Cluster(cv::Mat, int, int);

int main()
{
	// Input 이미지들 파일명이다.
    // 사용자에게 알맞는 이미지 파일로 이미지를 읽어드리면 된다.
	string CLUSTER[5] = { "/home/cona/algorithm_ws/src/Opencv/data/home.jpg", "/home/cona/algorithm_ws/src/Opencv/data/test1.jpg",
     "/home/cona/algorithm_ws/src/Opencv/data/test2.jpg", "/home/cona/algorithm_ws/src/Opencv/data/test3.jpg", "/home/cona/algorithm_ws/src/Opencv/data/test4.jpg" };
	string path = "/home/cona/algorithm_ws/src/Opencv/";

	// KMeans Clustering
	for (int i = 0; i < 5; i++) {
		cv::Mat img = cv::imread(CLUSTER[i]);
		cv::Mat result;
		int k = 8;
		result = KMeans_Cluster(img, k, 5);
		cv::imwrite(path + "Test_Result_KMeans/" + CLUSTER[i], result);
	}

	return 0;
}

cv::Mat KMeans_Cluster(cv::Mat src, int n_Cluster, int D = 5) {
	cv::Mat result = cv::Mat::zeros(src.size(), CV_8UC3);

	// Step1. 화소를 (b, g, r) 3차원 형태로 변환 시킴.
	uchar* sImage = src.data;
	vector <cv::Point3f> point;
	for (int r = 0; r < src.rows; r++) {
		for (int c = 0; c < src.cols; c++) {
			int offset = r * src.cols * 3 + c * 3;
			point.push_back(cv::Point3f(sImage[offset + 0], sImage[offset + 1], sImage[offset + 2]));
		}
	}

	// Step2. 초기 군집 중심 k를 설정
	// 2 가지 방법이 있는데, 초기화를 랜덤하게 하는 방법이 있고, 
	vector<cv::Point3f> center_point;
	srand((unsigned int)time(NULL));
	/*for (int i = 0; i < n_Cluster; i++) {
		int rand1, rand2, rand3;
		rand1 = rand() % 255, rand2 = rand() % 255, rand3 = rand() % 255;
		center_point.push_back(cv::Point3f(rand1, rand2, rand3));
		cout << rand1 << " " << rand2 << " " << rand3 << endl;
	}*/
	center_point.push_back(cv::Point3f(0, 0, 0));
	center_point.push_back(cv::Point3f(0, 0, 255));
	center_point.push_back(cv::Point3f(0, 255, 0));
	center_point.push_back(cv::Point3f(0, 255, 255));
	center_point.push_back(cv::Point3f(255, 0, 0));
	center_point.push_back(cv::Point3f(255, 0, 255));
	center_point.push_back(cv::Point3f(255, 255, 0));
	center_point.push_back(cv::Point3f(255, 255, 255));

	cout << center_point.size() << endl;

	// 군집에 속하는 포인트들을 누적합 하기 위한 vector 선언
	vector<vector<int>> calc(center_point.size());
	bool is_first = true;
	while (true) {
		bool is_changed = true;

		// Step3. 모든 point들을 한 바퀴 돌면서 가장 가까운 군집 중심에 배정
		for (int i = 0; i < point.size(); i++) {
			int min_dist = MAX_DIST;
			int min_center = -1;
			for (int k = 0; k < center_point.size(); k++) {
				int distance;
				distance = sqrt(pow(point[i].x - center_point[k].x, 2) + pow(point[i].y - center_point[k].y, 2) + pow(point[i].z - center_point[k].z, 2));
				if (distance < min_dist) {
					min_dist = distance;
					min_center = k; // 가장 가까운 군집 k를 기억
				}
			}
			calc[min_center].push_back(i); // 하나의 point에서 모든 군집 중심중에 가장 가까운 군집 중심에 point[i]가 속하게 된다.
		}

		// 현재 배정된 point들의 중점을 구하여 임시 중심점으로 저장
		vector<cv::Point3f> temp_center_point(center_point.size());
		for (int k = 0; k < calc.size(); k++) {
			int b = 0, g = 0, r = 0;
			for (int j = 0; j < calc[k].size(); j++) {
				b += point[calc[k][j]].x;
				g += point[calc[k][j]].y;
				r += point[calc[k][j]].z;
			}
			if (b == 0)
				b = 0;
			else if (g == 0)
				g = 0;
			else if (r == 0)
				r = 0;
			else {
				b /= calc[k].size();
				g /= calc[k].size();
				r /= calc[k].size();
			}

			temp_center_point[k].x = b;
			temp_center_point[k].y = g;
			temp_center_point[k].z = r;
		}

		// 만약 첫 번째 계산이었다면, center_point들은 랜덤이다.
		// 두 번째 계산 이상부터는 center_point와 현재 새롭게 계산된 새로운 중심점들인 temp_center_point 와 거리 차이를 계산해준다.
		int magChange = 0;
		for (int k = 0; k < calc.size(); k++) {
			magChange += sqrt(pow(temp_center_point[k].x - center_point[k].x, 2) + pow(temp_center_point[k].y - center_point[k].y, 2) + pow(temp_center_point[k].z - center_point[k].z, 2));
		}

		// 근데 중심점들끼리의 거리가 같을 수도 있다. 어느 정도 가까운 건 상관없지만, 너무 가깝거나 같은 경우에는 다시 루프를 돌게 해야한다.
		bool is_same = false;
		for (int k1 = 0; k1 < calc.size() - 1; k1++) {
			for (int k2 = k1 + 1; k2 < calc.size(); k2++) {
				if (sqrt(pow(temp_center_point[k1].x - temp_center_point[k2].x, 2) + pow(temp_center_point[k1].y - temp_center_point[k2].y, 2) + pow(temp_center_point[k1].z - temp_center_point[k2].z, 2)) <= 5) {
					k1 = calc.size();
					is_same = true;
					break;
				}					
			}
		}

		// Step4. 여기서 첫 배정이면 다음으로 패쓰, 첫 배정이 아니면 이전 루프의 배정과 같은지 비교
		if (is_first == true) {
			is_first = false;
			continue;
		}
		else if(magChange <= D &&  is_same == false){ // 현재 중심점들과 이전 중심점들과의 거리 차이가 D 이하면 종료한다.
			break;
		}
		else {
			for (int k = 0; k < center_point.size(); k++) {
				center_point[k].x = temp_center_point[k].x;
				center_point[k].y = temp_center_point[k].y;
				center_point[k].z = temp_center_point[k].z;
			}
		}
	}
	for (int k = 0; k < center_point.size(); k++)
		cout << "Final Ks : " << center_point[k].x << " " << center_point[k].y << " " << center_point[k].z << endl;
	
	// 최종 배정된 중심점들을 이용하여 색상 변환
	uchar* rImage = result.data;
	for (int k = 0; k < calc.size(); k++) {
		for (int i = 0; i < calc[k].size(); i++) {
			rImage[calc[k][i] * 3 + 0] = center_point[k].x;
			rImage[calc[k][i] * 3 + 1] = center_point[k].y;
			rImage[calc[k][i] * 3 + 2] = center_point[k].z;
		}
	}

	return result;
}