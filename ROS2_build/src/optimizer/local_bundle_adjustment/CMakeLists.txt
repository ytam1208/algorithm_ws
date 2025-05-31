cmake_minimum_required(VERSION 3.8)
project(local_bundle_adjustment)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
set(CMAKE_CXX_STANDARD 17)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Ceres REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(glog REQUIRED)

include_directories(
  ${EIGEN3_INCLUDE_DIRS}
  ${CERES_INCLUDE_DIRS}
  ${GLOG_INCLUDE_DIRS}
)

add_executable(local_bundle_adjustment
  src/local_bundle_adjustment.cpp
)
ament_target_dependencies(local_bundle_adjustment 
  rclcpp
)
target_link_libraries(local_bundle_adjustment
  ${CERES_LIBRARIES}
  ${EIGEN3_LIBRARIES}
  glog::glog
)

install(TARGETS local_bundle_adjustment
  DESTINATION lib/${PROJECT_NAME})

ament_package()
