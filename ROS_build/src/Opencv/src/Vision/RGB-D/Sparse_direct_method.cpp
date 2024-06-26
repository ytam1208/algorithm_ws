#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <Eigen/Dense>

#include <g2o/core/base_vertex.h>
#include <g2o/core/base_unary_edge.h>
#include <g2o/core/block_solver.h>
#include <g2o/core/optimization_algorithm_levenberg.h>
#include <g2o/core/optimization_algorithm_gauss_newton.h>
#include <g2o/solvers/dense/linear_solver_dense.h>
#include <g2o/types/sba/types_six_dof_expmap.h>

class Odom
{
public:
    Eigen::Isometry3d Tcw;
    double x,y,z,theta;
    inline Eigen::Isometry3d addMotion(Eigen::Isometry3d& _Tcw){
        return (_Tcw * Tcw);        
    }
    void Get_relative_pose(Eigen::Isometry3d& _Tcw){
        // Eigen::Quaterniond Rot(_Tcw.rotation());
        // Eigen::Isometry3d _Twr(Eigen::Quaterniond(Rot.w(), Rot.x(), Rot.y(), Rot.z()));
        // _Twr.pretranslate(Eigen::Vector3d(_Tcw.translation()(0), _Tcw.translation()(1), _Tcw.translation()(2)));
        
    }
    Odom(){
        // Eigen::Isometry3d _Twr(Eigen::Quaterniond(0.6078, -0.6810, -0.2887, 0.2888));
        // _Twr.pretranslate(Eigen::Vector3d(-1.8476, 2.9446, 0.5268));
        Eigen::Isometry3d _Twr(Eigen::Quaterniond(-0.3909, 0.8851, 0.2362, -0.0898));
        _Twr.pretranslate(Eigen::Vector3d(1.3112, 0.8507, 1.5186));
        Tcw = _Twr;
    }
    ~Odom(){}
};

// Save 3D world coordinate, grayscale
struct Measurement
{
    Measurement ( Eigen::Vector3d p, float g ) : pos_world ( p ), grayscale ( g ) {}
    Eigen::Vector3d pos_world;
    float grayscale;
};

inline Eigen::Vector3d project2Dto3D ( int x, int y, int d, float fx, float fy, float cx, float cy, float scale )
{
    float zz = float ( d ) /scale;
    float xx = zz* ( x-cx ) /fx;
    float yy = zz* ( y-cy ) /fy;
    return Eigen::Vector3d ( xx, yy, zz );
}

inline Eigen::Vector2d project3Dto2D ( float x, float y, float z, float fx, float fy, float cx, float cy )
{
    float u = fx*x/z+cx;
    float v = fy*y/z+cy;
    return Eigen::Vector2d ( u,v );
}

// Direct Pose Estimation
// 입력: 측정값(공간점의 그레이스케일), 새로운 그레이스케일, 카메라 내부파라미터
// 출력: 카메라의 위치
bool poseEstimationDirect ( const std::vector<Measurement>& measurements, cv::Mat* gray, Eigen::Matrix3f& intrinsics, Eigen::Isometry3d& Tcw );


// project a 3d point into an image plane, the error is photometric error
// an unary edge with one vertex SE3Expmap (the pose of camera)
class EdgeSE3ProjectDirect: public g2o::BaseUnaryEdge< 1, double, g2o::VertexSE3Expmap>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    EdgeSE3ProjectDirect() {}

    EdgeSE3ProjectDirect ( Eigen::Vector3d point, float fx, float fy, float cx, float cy, cv::Mat* image )
        : x_world_ ( point ), fx_ ( fx ), fy_ ( fy ), cx_ ( cx ), cy_ ( cy ), image_ ( image )
    {}

    virtual void computeError()
    {
        const g2o::VertexSE3Expmap* v  = static_cast<const g2o::VertexSE3Expmap*> ( _vertices[0] );
        Eigen::Vector3d x_local = v->estimate().map ( x_world_ );
        float x = x_local[0]*fx_/x_local[2] + cx_;
        float y = x_local[1]*fy_/x_local[2] + cy_;
        // check x,y is in the image
        if (x-4 < 0 
            || (x+4) > image_->cols 
            || (y-4) < 0 
            || (y+4) > image_->rows)
        {
            _error ( 0,0 ) = 0.0;
            this->setLevel ( 1 );
        }
        else
        {
            _error ( 0,0 ) = getPixelValue ( x,y ) - _measurement;
        }
    }

    // plus in manifold
    virtual void linearizeOplus( )
    {
        if ( level() == 1 )
        {
            _jacobianOplusXi = Eigen::Matrix<double, 1, 6>::Zero();
            return;
        }
        g2o::VertexSE3Expmap* vtx = static_cast<g2o::VertexSE3Expmap*> ( _vertices[0] );
        Eigen::Vector3d xyz_trans = vtx->estimate().map ( x_world_ );   // q in book

        double x = xyz_trans[0];
        double y = xyz_trans[1];
        double invz = 1.0/xyz_trans[2];
        double invz_2 = invz*invz;

        float u = x*fx_*invz + cx_;
        float v = y*fy_*invz + cy_;

        // jacobian from se3 to u,v
        // NOTE that in g2o the Lie algebra is (\omega, \epsilon), where \omega is so(3) and \epsilon the translation
        Eigen::Matrix<double, 2, 6> jacobian_uv_ksai;

        jacobian_uv_ksai ( 0,0 ) = - x*y*invz_2 *fx_;
        jacobian_uv_ksai ( 0,1 ) = ( 1+ ( x*x*invz_2 ) ) *fx_;
        jacobian_uv_ksai ( 0,2 ) = - y*invz *fx_;
        jacobian_uv_ksai ( 0,3 ) = invz *fx_;
        jacobian_uv_ksai ( 0,4 ) = 0;
        jacobian_uv_ksai ( 0,5 ) = -x*invz_2 *fx_;

        jacobian_uv_ksai ( 1,0 ) = - ( 1+y*y*invz_2 ) *fy_;
        jacobian_uv_ksai ( 1,1 ) = x*y*invz_2 *fy_;
        jacobian_uv_ksai ( 1,2 ) = x*invz *fy_;
        jacobian_uv_ksai ( 1,3 ) = 0;
        jacobian_uv_ksai ( 1,4 ) = invz *fy_;
        jacobian_uv_ksai ( 1,5 ) = -y*invz_2 *fy_;

        Eigen::Matrix<double, 1, 2> jacobian_pixel_uv;

        jacobian_pixel_uv ( 0,0 ) = ( getPixelValue ( u+1,v )-getPixelValue ( u-1,v ) ) /2;
        jacobian_pixel_uv ( 0,1 ) = ( getPixelValue ( u,v+1 )-getPixelValue ( u,v-1 ) ) /2;

        _jacobianOplusXi = jacobian_pixel_uv*jacobian_uv_ksai;
    }

    // dummy read and write functions because we don't care...
    virtual bool read ( std::istream& in ) {return true;}
    virtual bool write ( std::ostream& out ) const {return true;}

protected:
    // get a gray scale value from reference image (bilinear interpolated)
    inline float getPixelValue ( float x, float y )
    {
        uchar* data = & image_->data[ int ( y ) * image_->step + int ( x ) ];
        float xx = x - floor ( x );
        float yy = y - floor ( y );
        return float (
                   ( 1-xx ) * ( 1-yy ) * data[0] +
                   xx* ( 1-yy ) * data[1] +
                   ( 1-xx ) *yy*data[ image_->step ] +
                   xx*yy*data[image_->step+1]
               );
    }
public:
    Eigen::Vector3d x_world_;   // 3D point in world frame
    float cx_=0, cy_=0, fx_=0, fy_=0; // Camera intrinsics
    cv::Mat* image_=nullptr;    // reference image
};

Odom om;
int main ( int argc, char** argv )
{
    srand ( ( unsigned int ) time ( 0 ) );
    std::string path_to_dataset = "/home/cona/github/algorithm_ws/ROS_build/data";
    // std::string path_to_dataset = "/home/cona/rgbd_dataset_freiburg2_pioneer_slam3";
    std::string associate_file = path_to_dataset + "/associate.txt";

    std::ifstream fin ( associate_file );

    std::string rgb_file, depth_file, time_rgb, time_depth;
    cv::Mat color, depth, gray;
    std::vector<Measurement> measurements;
    std::vector<Measurement> OB_measurements;
    
    // 카메라 내부변수
    float cx = 325.5;
    float cy = 253.5;
    float fx = 518.0;
    float fy = 519.0;
    // float cx = 319.5;
    // float cy = 239.5;
    // float fx = 525.0;
    // float fy = 525.0;
    float depth_scale = 1000.0;
    Eigen::Matrix3f K;  
    K<<fx,0.f,cx,0.f,fy,cy,0.f,0.f,1.0f;


    Eigen::Isometry3d Tcw = Eigen::Isometry3d::Identity();
    cv::Mat prev_color;
    // 첫번째 영상을 참고하여, 다음 obervation 영상과 reference 영상을 처리했다.
    for (int index = 0; index < 10; index++)
    {
        std::cout << "*********** loop "<<index<<" ************" << std::endl;
        fin >> time_rgb >> rgb_file >> time_depth >> depth_file;
        color = cv::imread(path_to_dataset + "/" + rgb_file);
        depth = cv::imread(path_to_dataset + "/" + depth_file, -1);
        if (color.data==nullptr || depth.data==nullptr)
            continue; 
        cv::cvtColor (color, gray, cv::COLOR_BGR2GRAY);
        OB_measurements.clear();
        if (index ==0)
        {
            measurements.clear();
            // select the pixels with high gradiants 
            for (int x=10; x<gray.cols-10; x++)
                for (int y=10; y<gray.rows-10; y++)
                {
                    Eigen::Vector2d delta (
                        gray.ptr<uchar>(y)[x+1] - gray.ptr<uchar>(y)[x-1], 
                        gray.ptr<uchar>(y+1)[x] - gray.ptr<uchar>(y-1)[x]
                    );
                    if (delta.norm() < 50)
                        continue;
                    ushort d = depth.ptr<ushort> (y)[x];
                    if ( d==0 )
                        continue;
                    Eigen::Vector3d p3d = project2Dto3D ( x, y, d, fx, fy, cx, cy, depth_scale );
                    // std::cout << "[" << d << ", " << p3d << "]";
                    float grayscale = float ( gray.ptr<uchar> (y) [x] );
                    measurements.push_back ( Measurement ( p3d, grayscale ) );
                }
            prev_color = color.clone();
            // std::cout << "add total " << measurements.size() << " measurements." << std::endl;
            continue;
        }

        for (int x=10; x<gray.cols-10; x++)
            for (int y=10; y<gray.rows-10; y++)
            {
                Eigen::Vector2d delta (
                    gray.ptr<uchar>(y)[x+1] - gray.ptr<uchar>(y)[x-1], 
                    gray.ptr<uchar>(y+1)[x] - gray.ptr<uchar>(y-1)[x]
                );
                if (delta.norm() < 50)
                    continue;
                ushort d = depth.ptr<ushort> (y)[x];
                if ( d==0 )
                    continue;
                Eigen::Vector3d p3d = project2Dto3D ( x, y, d, fx, fy, cx, cy, depth_scale );
                // std::cout << "[" << d << ", " << p3d << "]";
                float grayscale = float ( gray.ptr<uchar> (y) [x] );
                OB_measurements.push_back ( Measurement ( p3d, grayscale ) );
            }

        // 카메라 움직임 계산하기
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        poseEstimationDirect (measurements, &gray, K, Tcw);
        std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
        std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>> ( t2-t1 );
        std::cout << "direct method costs time: " << time_used.count() << " seconds." << std::endl;
        // std::cout << "Tcw="<<Tcw.matrix() << std::endl;

        // plot the feature points
        cv::Mat img_show ( color.rows*2, color.cols, CV_8UC3 );
        prev_color.copyTo ( img_show ( cv::Rect ( 0,0,color.cols, color.rows ) ) );
        color.copyTo ( img_show ( cv::Rect ( 0,color.rows,color.cols, color.rows ) ) );
        
        for ( Measurement m:measurements )
        {
            // if ( rand() > RAND_MAX / 5 )
            //     continue;
            Eigen::Vector3d p = m.pos_world;
            Eigen::Vector2d pixel_prev = project3Dto2D ( p ( 0,0 ), p ( 1,0 ), p ( 2,0 ), fx, fy, cx, cy );
            Eigen::Vector3d p2 = Tcw*m.pos_world;
            Eigen::Vector2d pixel_now = project3Dto2D ( p2 ( 0,0 ), p2 ( 1,0 ), p2 ( 2,0 ), fx, fy, cx, cy );
            if (pixel_now(0,0) < 0 || pixel_now(0,0) >= color.cols || pixel_now(1,0) <0 || pixel_now(1,0) >= color.rows)
                continue;

            float b = 0;
            float g = 250;
            float r = 0;
            img_show.ptr<uchar>( pixel_prev(1,0) )[int(pixel_prev(0,0))*3] = b;
            img_show.ptr<uchar>( pixel_prev(1,0) )[int(pixel_prev(0,0))*3+1] = g;
            img_show.ptr<uchar>( pixel_prev(1,0) )[int(pixel_prev(0,0))*3+2] = r;
            
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3] = b;
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3+1] = g;
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3+2] = r;
            cv::circle ( img_show, cv::Point2d ( pixel_prev ( 0,0 ), pixel_prev ( 1,0 ) ), 2, cv::Scalar ( b,g,r ), 2 );
            cv::circle ( img_show, cv::Point2d ( pixel_now ( 0,0 ), pixel_now ( 1,0 ) +color.rows ), 2, cv::Scalar ( 250,g,r ), 2 );
        }

        for ( Measurement m:OB_measurements )
        {
            Eigen::Vector3d p2 = m.pos_world;
            Eigen::Vector2d pixel_now = project3Dto2D ( p2 ( 0,0 ), p2 ( 1,0 ), p2 ( 2,0 ), fx, fy, cx, cy );
            if (pixel_now(0,0) < 0 || pixel_now(0,0) >= color.cols || pixel_now(1,0) <0 || pixel_now(1,0) >= color.rows)
                continue;

            float b = 0;
            float g = 250;
            float r = 0;
            
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3] = b;
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3+1] = g;
            img_show.ptr<uchar>( pixel_now(1,0)+color.rows )[int(pixel_now(0,0))*3+2] = r;
            cv::circle ( img_show, cv::Point2d ( pixel_now ( 0,0 ), pixel_now ( 1,0 ) +color.rows ), 0.5, cv::Scalar ( b,0,255 ), 2 );
        }
        Eigen::Isometry3d Motion = Tcw.inverse();
        Eigen::Quaterniond Rot_M(Motion.rotation());

        std::cout << "******************************" << std::endl;
        std::cout << "********inverse_First*********" << std::endl;
        std::cout << "******************************" << std::endl;
        std::cout << "T= \n" << Motion.translation()(0) << ", " << Motion.translation()(1) << ", " << Motion.translation()(2) << 
        ", " << Rot_M.x() << ", " << Rot_M.y() << ", " << Rot_M.z() << ", " << Rot_M.w() << std::endl;

        auto euler = Rot_M.toRotationMatrix().eulerAngles(0, 1, 2);
        std::cout << "Roll= " << euler.x()*(M_PI / 180.0) << " Pitch= " << euler.y()*(M_PI / 180.0) << " yaw= " << euler.z()*(M_PI / 180.0) << std::endl;

        Eigen::Isometry3d N_D = om.addMotion(Motion);
        Eigen::Quaterniond Rot_M4(N_D.rotation());
        std::cout << "******************************" << std::endl;
        std::cout << "************Motion************" << std::endl;
        std::cout << "******************************" << std::endl;
        std::cout << "T= \n" << N_D.translation()(0) << ", " << N_D.translation()(1) << ", " << N_D.translation()(2) << 
        ", " << Rot_M4.x() << ", " << Rot_M4.y() << ", " << Rot_M4.z() << ", " << Rot_M4.w() << std::endl;
        
        // auto euler2 = Rot_M.toRotationMatrix().eulerAngles(0, 1, 2);
        // std::cout << "Roll= " << euler2.x()*(M_PI / 180.0) << " Pitch= " << euler2.y()*(M_PI / 180.0) << " yaw= " << euler2.z()*(M_PI / 180.0) << std::endl;
       
        // std::cout << "******************************" << std::endl;
        // std::cout << "R= \n" << Rot_M.x() << ", " << Rot_M.y() << ", " << Rot_M.z() << ", " << Rot_M.w() << std::endl;

        cv::imshow ( "result", img_show );
        cv::waitKey ( 0 );

    }
    return 0;
}
//https://alida.tistory.com/16
bool poseEstimationDirect ( const std::vector< Measurement >& measurements, cv::Mat* gray, Eigen::Matrix3f& K, Eigen::Isometry3d& Tcw )
{
    // LinearSolver는 Pose(6), Landmark(1)의 문제를 풀때 BlockSolver 설정
    typedef g2o::BlockSolver<g2o::BlockSolverTraits<6,1>> DirectBlock;  
    DirectBlock::LinearSolverType* linearSolver = new g2o::LinearSolverDense< DirectBlock::PoseMatrixType > ();
    DirectBlock* solver_ptr = new DirectBlock ( linearSolver );
    // g2o::OptimizationAlgorithmGaussNewton* solver = new g2o::OptimizationAlgorithmGaussNewton( std::move(solver_ptr) ); // G-N
    // Optimizer는 Levenberg-Marquardt 방법 사용, Lambda 초기값 설정
    g2o::OptimizationAlgorithmLevenberg* solver = new g2o::OptimizationAlgorithmLevenberg ( std::move(solver_ptr)); // L-M

    g2o::SparseOptimizer optimizer;
    optimizer.setAlgorithm ( solver );
    // 최적화 과정을 프린트할지(verbose) 설정한다
    // optimizer.setVerbose( true );

    //g2o optimizer에 추가할 g2o::VertexSE3Expmap 노드를 생성한다
    //보통 original pose의 벡터 크기만큼 루프를 돌면서 g2o optimizer에 추가할 g2o::VertexSE3Expmap 노드를 생성한다. 
    g2o::VertexSE3Expmap* pose = new g2o::VertexSE3Expmap();
    // g2o::SE3Quat 자료구조에 초기 pose로 선언을 하며, Tcw 카메라 pose를 넣는다.
    pose->setEstimate (g2o::SE3Quat(Tcw.rotation(), Tcw.translation()));
    //이 예제에서는 1개의 Reference이기 때문에 0으로 하나만 셋팅
    pose->setId (0);
    // 첫번째 노드인 경우 해당 노드(또는 vertex)의 포즈를 고정시킨다.
    // if(i==0)
    //     pose->setFixed(true);
    optimizer.addVertex ( pose );

    // 添加边
    int id=1;
    for ( Measurement m: measurements )
    {
        EdgeSE3ProjectDirect* edge = new EdgeSE3ProjectDirect (
            m.pos_world,
            K ( 0,0 ), K ( 1,1 ), K ( 0,2 ), K ( 1,2 ), gray
        );
        edge->setVertex ( 0, pose );
        edge->setMeasurement ( m.grayscale );
        edge->setInformation ( Eigen::Matrix<double,1,1>::Identity() );
        edge->setId ( id++ );
        optimizer.addEdge ( edge );
    }
    // std::cout << "edges in graph: "<< optimizer.edges().size() << std::endl;
    optimizer.initializeOptimization();
    optimizer.optimize (100);
    Tcw = pose->estimate();
    return true;
}

/*
1305031453.3595 1.3112 0.8507 1.5186 0.8851 0.2362 -0.0898 -0.3909

1305031453.3995 1.3212 0.8419 1.5212 0.8885 0.2232 -0.0896 -0.3908
1305031453.4295 1.3291 0.8350 1.5236 0.8889 0.2134 -0.0896 -0.3955
1305031453.4595 1.3370 0.8274 1.5265 0.8893 0.2037 -0.0885 -0.3998
1305031453.4995 1.3480 0.8158 1.5311 0.8900 0.1906 -0.0879 -0.4048
1305031453.5295 1.3555 0.8063 1.5349 0.8886 0.1824 -0.0894 -0.4112
1305031453.5595 1.3627 0.7957 1.5391 0.8865 0.1763 -0.0915 -0.4180
1305031453.5995 1.3712 0.7805 1.5448 0.8838 0.1699 -0.0932 -0.4258
1305031453.6295 1.3762 0.7680 1.5488 0.8813 0.1690 -0.0962 -0.4308
*/