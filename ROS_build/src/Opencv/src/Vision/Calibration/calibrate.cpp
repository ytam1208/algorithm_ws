#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
/*************************************************** */
string window_user = "/home/cona";
string mac_user = "/Users/yeontaemin";

string folder_path = window_user + "/github/algorithm_ws/ROS_build/src/Opencv/src/Vision/Calibration/chessboard/";

string left_img = folder_path + "left01.jpg";
/*************************************************** */

/*************************************************** */
/*                체스보드 파라미터                   */
/*************************************************** */
std::vector<cv::String> images;
std::vector<cv::Point3f> obj; //체스보드 3D points

std::vector<std::vector<cv::Point3f>> objpoints; //프레임 단위에서의 체스보드 3D points
std::vector<std::vector<cv::Point2f>> imgpoints; //체스보드에서 찾은 2D points		 

int boardHeight = 9;
int boardWidth = 6;
cv::Size boardSize = cv::Size(boardHeight, boardWidth);
/*************************************************** */

void initChessboard(void)
{
  // 3D 포인트 생성 (체커보드 평면상의 좌표)
  float chess_distance = 40.0;        //mm
  for(int y=0; y<boardHeight; y++)    //체커보드 9 rows
  {
    for(int x=0; x<boardWidth; x++)   //체커보드 6 cols
    {
      obj.push_back(cv::Point3f(x*chess_distance, y*chess_distance, 0));
    }
  }
}

void calcChessboard(cv::Mat& img)
{
  cv::Mat gray;
  cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

  std::vector<cv::Point2f> corners;
  bool found = findChessboardCorners(gray, boardSize, corners, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);
  if(found)
  {
    cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

    cv::cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1), criteria);

    cv::drawChessboardCorners(gray, boardSize, corners, found);
    imgpoints.push_back(corners);
    objpoints.push_back(obj);    
    
    // cv::imshow("gray_l", gray);
    // cv::waitKey(0);
  }
  else{std::cout << "not found" << std::endl;}
}

void calibration(void)
{
  // 카메라 매트릭스와 왜곡 계수 초기화
  cv::Mat cameraMatrix;
  cv::Mat distCoeffs;
  std::vector<cv::Mat> rvecs, tvecs;

  std::cout << "corner point = " << imgpoints.back().size() << std::endl;
  std::cout << "board point = " << objpoints.back().size() << std::endl;

  // 카메라 캘리브레이션
  cv::calibrateCamera(objpoints, imgpoints, boardSize, cameraMatrix, distCoeffs, rvecs, tvecs);
  
  // 결과 출력
  cout << "Camera Matrix: \n" << cameraMatrix << endl;
  cout << "Distortion Coefficients: \n" << distCoeffs << endl;
}

int main(int argc, char **argv) 
{
  cv::glob(folder_path, images);
  std::cout << "number of image [" << images.size() << "]" << std::endl;
  if(images.size()==0){return -1;}

  initChessboard();
  for(int i=0; i<images.size(); i++)
  {
    cv::Mat frame = cv::imread(images[i]);
    if(frame.empty()==true){continue;}
    calcChessboard(frame);
  }

  calibration();
  
  return 0;
}