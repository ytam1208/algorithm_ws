cmake_minimum_required(VERSION 3.10)
project(code_test)

include_directories(
 include
 ${OpenCV_INCLUDE_DIRS}
#  ${CERES_INCLUDE_DIRS}
)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
include_directories( "/usr/include/eigen3" )

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

add_executable(DiscreteBayesFilter src/SLAM/DiscreteBayesFilter.cpp)

add_executable(Dummy_HeatMap src/SLAM/Dummy_HeatMap.cpp)

add_executable(eigendecomposition src/linear_algebra/eigendecomposition.cpp)

add_executable(pca src/SLAM/PCA.cpp)
target_link_libraries(pca
  ${OpenCV_LIBRARIES}
)

add_executable(normalvector src/SLAM/Get_normalVector.cpp)
target_link_libraries(pca
  ${Eigen_LIBRARIES}
)
