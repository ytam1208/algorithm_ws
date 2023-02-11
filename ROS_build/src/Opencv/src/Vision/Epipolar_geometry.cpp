// https://alida.tistory.com/59

#include <Opencv/Opencv_tuto.hpp>
#include <Opencv/read_path.hpp>

class Get_data
{
    private:
        Path::Loader get_path;
        Display::Lodar check_mat;
        std::string L_path;
        std::string R_path;
        cv::Mat L_src, R_src;

    public:
        bool operator()(cv::Mat& inputL_mat, cv::Mat& inputR_mat){

            inputL_mat = L_src;
            inputR_mat = R_src;
            return 1;
        }
        Get_data(){
            L_path = get_path("mac") + "aloeL.jpg";
            R_path = get_path("mac") + "aloeR.jpg";
            L_src = cv::imread(L_path, cv::IMREAD_GRAYSCALE);
            R_src = cv::imread(R_path, cv::IMREAD_GRAYSCALE);

            if(check_mat(L_src) && check_mat(R_src)){
                // cv::imshow("aloeL", L_src);
                // cv::imshow("aloeR", R_src);
                // cv::waitKey(0);
            }
            else    
                std::cout << "No img" << std::endl;
        }
        ~Get_data(){}
};

class ORB_feature
{
    private:
        cv::Mat key_L, key_R;
        cv::Mat L_ptr, R_ptr; 
        std::vector<cv::KeyPoint> L_point, R_point;

        void extract_ORB_feature(){
            cv::imshow("aloeL", L_ptr);
            cv::imshow("aloeR", R_ptr);
            cv::waitKey(0);

            cv::Ptr<cv::Feature2D> orb = cv::ORB::create(); // const static auto&  
            orb->detectAndCompute(L_ptr, cv::noArray(), L_point, key_L);
            orb->detectAndCompute(R_ptr, cv::noArray(), R_point, key_R);
        };
        void matches_ORB_feature(){
            cv::Ptr<cv::DescriptorMatcher> matcher = cv::BFMatcher::create(cv::NORM_HAMMING);
            std::vector<cv::DMatch> matches;
            matcher->match(key_L, key_R, matches);
            std::sort(matches.begin(), matches.end());
            std::vector<cv::DMatch> good_matches(matches.begin(), matches.begin() + 100);

            cv::Mat dst;
            cv::drawMatches(L_ptr, L_point, R_ptr, R_point, good_matches, dst, cv::Scalar::all(-1), cv::Scalar::all(-1), std::vector<char>());
            cv::drawKeypoints(L_ptr, L_point, L_ptr, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
            cv::drawKeypoints(R_ptr, R_point, R_ptr, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

            cv::imshow("dst", dst);
            cv::imshow("L_ptr", L_ptr);
            cv::imshow("R_ptr", R_ptr);

            cv::Mat fundamental_matrix = cv::findFundamentalMat(L_point, R_point, FM_8POINT);
            for(auto iter : L_point){

                cv::computeCorrespondEpilines(iter.pt, 1, fundamental_matrix);

            }

            cv::waitKey(0);
        }
    public:
        ORB_feature(Get_data gd){
            gd(L_ptr, R_ptr);
            extract_ORB_feature();
            matches_ORB_feature();
        }
        ~ORB_feature(){}
};

int main()
{
    Get_data gd;
    ORB_feature of(gd);

    return 0;
}