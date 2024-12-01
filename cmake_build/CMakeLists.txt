cmake_minimum_required(VERSION 3.10)
project(code_test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories("/usr/include/eigen3")
include_directories(
 include
 ${OpenCV_INCLUDE_DIRS}
 ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/
 #  ${CERES_INCLUDE_DIRS}
 )

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED) 

# add_subdirectory(abseil-cpp)
# add_executable(log src/abseil/log.cpp)  
# target_link_libraries(log
#   ${Eigen3_LIBS}
#   ${OpenCV_LIBRARIES}
#   absl::log
#   absl::strings
# )

# add_executable(flat_hash_set src/abseil/flat_hash_set.cpp)
# target_link_libraries(flat_hash_set
#   ${Eigen3_LIBS}
#   ${OpenCV_LIBRARIES}
#   absl::log
#   absl::strings
#   absl::flat_hash_set
# )

# add_executable(flat_hash_map src/abseil/flat_hash_map.cpp)
# target_link_libraries(flat_hash_map
#   ${Eigen3_LIBS}
#   ${OpenCV_LIBRARIES}
#   absl::log
#   absl::strings
#   absl::flat_hash_map
# )

# add_executable(test src/main.cpp)

# add_executable(inline src/inline.cpp)

# add_executable(class_new src/class_new.cpp)

# add_executable(virtual_class src/virtual_class.cpp)

# add_executable(unique_ptr src/unique_ptr.cpp)

# add_executable(shared_ptr src/shared_ptr.cpp)

# add_executable(using_function_pointer src/using_function_pointer.cpp)

# add_executable(while_loop src/while_loop_clock_test.cpp)

# add_executable(shrink src/shrink_to_fit.cpp)

# add_executable(auto src/auto.cpp)

# add_executable(while src/while_loop_clock_test.cpp)

# add_executable(bind src/bind.cpp)

# add_executable(function src/function.cpp)

# add_executable(list src/list.cpp)

# add_executable(unordered_map src/unordered_map.cpp)

# add_executable(Naive_Singleton src/Morden_Design/Naive_Singleton.cpp)

# add_executable(Smart_Singleton src/Morden_Design/Smart_Singleton.cpp)

# add_executable(union src/union.cpp)

# add_executable(move src/move.cpp)

# add_executable(forward src/forward.cpp)

# add_executable(lambda src/lambda.cpp)

# add_executable(array src/array.cpp)

# add_executable(binary_search src/binary_search.cpp)

# add_executable(DiscreteBayesFilter src/SLAM/DiscreteBayesFilter.cpp)

# add_executable(Dummy_HeatMap src/SLAM/Dummy_HeatMap.cpp)

# add_executable(eigendecomposition src/linear_algebra/eigendecomposition.cpp)

# add_executable(pca src/SLAM/PCA.cpp)
# target_link_libraries(pca
#   ${OpenCV_LIBRARIES}
# )

# add_executable(orthogonal_least_square src/linear_algebra/orthogonal_least_square.cpp)
# target_link_libraries(orthogonal_least_square
#   ${OpenCV_LIBRARIES}
# )

# add_executable(Point_to_line_eigen src/linear_algebra/Point_to_line_eigen.cpp)
# target_link_libraries(Point_to_line_eigen
#   ${OpenCV_LIBRARIES}
# )

# add_executable(relative_pose src/linear_algebra/relative_pose.cpp)

# add_executable(condition_variable src/Multi_thread/condition_variable.cpp)

# add_executable(1D-kalmanFilter src/SLAM/kalmanFilter/1D_example.cpp)
# target_link_libraries(1D-kalmanFilter
#   ${Eigen3_LIBS}
# )

# add_executable(Kalman_example1 src/SLAM/kalmanFilter/example/Kalman_example1.cpp)
# target_link_libraries(Kalman_example1
#   ${Eigen3_LIBS}
# )

# add_executable(EKF_example1 src/SLAM/kalmanFilter/example/EKF_example1.cpp)
# target_link_libraries(EKF_example1
#   ${Eigen3_LIBS}
# )

add_executable(EKF_example2 src/SLAM/kalmanFilter/example/EKF_example2.cpp)
target_link_libraries(EKF_example2
  ${Eigen3_LIBS}
)