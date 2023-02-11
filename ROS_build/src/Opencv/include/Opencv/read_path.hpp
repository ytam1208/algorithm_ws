namespace Path
{
    class Loader
    {
    private:
        std::string for_mac = "/Users/yeontaemin/github/algorithm_ws/ROS_build/src/Opencv/data/";
        std::string for_linux = "/home/cona/github/algorithm_ws/ROS_build/src/Opencv/data/";
    public:
        std::string operator()(std::string input_data)
        {
            if(input_data.compare("mac")==0){
                std::cout << "Load -> /Users/yeontaemin/github/algorithm_ws/ROS_build/src/Opencv/data/" << std::endl;
                return for_mac;
            }
            else if(input_data.compare("linux")==0 || input_data.compare("window")==0){
                std::cout << "Load -> /home/cona/github/algorithm_ws/ROS_build/src/Opencv/data/" << std::endl;
                return for_linux;
            }
            else{
                std::cout << "Error Path" << std::endl;
                return "Error Path";    
            } 
        }
    public:
        Loader(){};
        ~Loader(){};
    };
}