#include "system.hpp"

class node
{
    public:
        int idx = 0;
        double x = 0.0;
        double y = 0.0;
        double th = 0.0;
        node(){}
        ~node(){}
};


int main()
{
    std::string file_path = "/home/cona/git/Graph-SLAM/Graph_slam/data_map/Map1/lm_data.dat";
    std::ifstream DBloader(file_path, std::ios_base::in | std::ios_base::binary);

    if(!DBloader.is_open()) return -1;

    int DB_size = 0;
    int idx(0);
    double _odom[3];
    DBloader.read((char*)&DB_size, sizeof(int));
    std::cout << "DB size = " << DB_size << std::endl;
    int loop_size;
    for(int i = 0; i < DB_size; i++){
        DBloader.read((char *)&idx, sizeof(int));
        DBloader.read((char *)_odom, sizeof(double) * 3);
        if(isnan(idx) != 0) idx = 0; 
        if(isfinite(idx))
            std::cout << "idx = " << idx << ", " << _odom[0] << ", " << _odom[1] << ", " << _odom[2] << ", " << std::endl;

        int row, col;
        DBloader.read((char*)&col, sizeof(int));
        DBloader.read((char*)&row, sizeof(int));
        if(col == 0 && row == 0)    return false;
        // std::cout << col << ", " << row << std::endl;
        // cv::Mat Points = cv::Mat(row, col, CV_32FC1);
        // DBloader.read((char*)Points.data, sizeof(float)*col*row); 

        // cv::Point2f laser_pt(0.0f, 0.0f);
        // loop_size = Points.rows;
        // for(int i = 0; i < loop_size; i++){
        //     laser_pt.x = Points.at<float>(i, 0);
        //     laser_pt.y = Points.at<float>(i, 1);
        //     // std::cout << "size = " << loop_size << ", " << laser_pt.x << ", " << laser_pt.y << std::endl;
        // }
    }  
    DBloader.close();

    return 0;
}