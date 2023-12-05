#include <iostream>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>

int main()
{
	std::vector<cv::Point2d> data_stream;

	data_stream.push_back(cv::Point2d(-2.94047, 0.638438)); //1391 
	data_stream.push_back(cv::Point2d(-2.92871, 0.622515)); //1392
	data_stream.push_back(cv::Point2d(-2.93938, 0.61139));  //1394
	data_stream.push_back(cv::Point2d(-3.00488, 0.611347)); //1395
	data_stream.push_back(cv::Point2d(-2.98412, 0.593577)); //1395
	data_stream.push_back(cv::Point2d(-2.98388, 0.580005)); //1396

    double w1 = 0,w2 = 0,w3 = 0;
    double mid1 = 0;
	double mid2 = 0;
	double mid3 = 0;
	double mid4 = 0; 
	double mid5 = 0;
    for(int iter=0; iter<data_stream.size(); iter++){
        mid1+=data_stream[iter].x;
        mid2+=data_stream[iter].y;
        mid3+=(data_stream[iter].x * data_stream[iter].x);
        mid4+=(data_stream[iter].y * data_stream[iter].y);
        mid5+=(data_stream[iter].x * data_stream[iter].y);
    }
    double size = 6.0;
	w1 = (size*mid5) - (mid1*mid2);
	w2 = (mid2*mid2) - (size*mid4) - (mid1*mid1) + (size*mid3);
	w3 = (mid1*mid2) - (size*mid5);

    double a,b,c;
	if(w1==0)
	{
		a = -1.0;
		b = 0.0;
		c = mid1/size;
	}
	else
	{
		a = (-w2+sqrt(w2*w2-4.0*w1*w3))/2.0/w1; 
		b = -1.0;							   
		c = (mid2-a*mid1)/size;			   
	}
    std::cout << "mean x = " << mid1 / size << " mean y = " << mid2 / size << std::endl;
    std::cout << "a= " << a << ", b= " << b << ", c= " << c << std::endl; 

    double distance;
    for(int iter=0; iter<data_stream.size(); iter++){
        distance = fabs(a*data_stream[iter].x + b*data_stream[iter].y + c) / (sqrt(pow(a,2) + 1.0));
        std::cout << distance << ", ";
    }
    std::cout <<std::endl;

    cv::Point2d p_pt = cv::Point2d(0.0, 0.0);
    std::function<double(double)> DEG2RAD = [](double radian){return radian * CV_PI/180.0;};

    double angle_start = -3.14159;
    double angle_increment = DEG2RAD(0.25); //0.25 
    double first_data_idx = 1391;
    for(int iter=0; iter<data_stream.size(); iter++){
        double theta = (first_data_idx+(double)iter) * angle_increment + angle_start;
        std::cout << "theta = " << theta << std::endl;
        if(fabs((fabs(theta) - CV_PI/2))<1e-05){
            p_pt.x = 0.0; 
            p_pt.y = c;
        }
        else{
            p_pt.x = (-c*cos(theta)) / (a*cos(theta) + b*sin(theta));
            p_pt.y = (-c*sin(theta)) / (a*cos(theta) + b*sin(theta));
        }
        std::cout << "PT(" << p_pt.x << ", " << p_pt.y << ")" << std::endl;
        
        cv::Point2d delta_pt;
        delta_pt.x = data_stream[iter].x - p_pt.x;
        delta_pt.y = data_stream[iter].y - p_pt.y;

        distance = sqrt(pow(delta_pt.x, 2)+pow(delta_pt.y, 2));
        std::cout << "Dis = " << distance << std::endl;
    }
    
    cv::Point2f End_point1;
    End_point1.x = (pow(b,2)*data_stream[0].x - a*b*data_stream[0].y - a*c) / (pow(a,2)+pow(b,2));
    End_point1.y = (pow(a,2)*data_stream[0].y - a*b*data_stream[0].x - b*c) / (pow(a,2)+pow(b,2));

    cv::Point2f End_point2;
    End_point2.x = (pow(b,2)*data_stream[data_stream.size()-1].x - a*b*data_stream[data_stream.size()-1].y - a*c) / (pow(a,2)+pow(b,2));
    End_point2.y = (pow(a,2)*data_stream[data_stream.size()-1].y - a*b*data_stream[data_stream.size()-1].x - b*c) / (pow(a,2)+pow(b,2));

    std::cout << "EndPoint1 = [" << End_point1.x << ", " << End_point1.y << "]" << std::endl;
    std::cout << "EndPoint2 = [" << End_point2.x << ", " << End_point2.y << "]" << std::endl;

    cv::Mat Grid(500, 500, CV_8UC3, cv::Scalar(255, 255, 255));	
    cv::Point2d grid_pt, data;
    int cnt=0;
    std::for_each(data_stream.begin(), data_stream.end(), [&](cv::Point2d& pt){

        data.x = (pt.x - mid1/size) * 200.0;
        data.y = (pt.y - mid2/size) * 200.0;

        grid_pt.x = 250.0 - data.y;
        grid_pt.y = 250.0 - data.x;

        if(cnt==0)
            cv::circle(Grid, grid_pt, 2, cv::Scalar(0, 0, 255), -1);
        else if(cnt==size-1)
            cv::circle(Grid, grid_pt, 2, cv::Scalar(0, 255, 0), -1);
        else
            cv::circle(Grid, grid_pt, 2, cv::Scalar(0, 0, 0), -1);

        data.x = (End_point1.x - mid1/size) * 200.0;
        data.y = (End_point1.y - mid2/size) * 200.0;
        grid_pt.x = 250.0 - data.y;
        grid_pt.y = 250.0 - data.x;
        cv::circle(Grid, grid_pt, 2, cv::Scalar(0, 255, 255), -1);

        data.x = (End_point2.x - mid1/size) * 200.0;
        data.y = (End_point2.y - mid2/size) * 200.0;
        grid_pt.x = 250.0 - data.y;
        grid_pt.y = 250.0 - data.x;
        cv::circle(Grid, grid_pt, 2, cv::Scalar(0, 255, 255), -1);

        cnt++;
    });

    cv::imshow("Dummy", Grid);
    cv::waitKey(0);
    return 0;
}