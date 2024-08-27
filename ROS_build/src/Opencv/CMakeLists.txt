cmake_minimum_required(VERSION 3.0.2)
project(Opencv)

## Compile as C++11, supported in ROS Kinetic and newer
# set(CMAKE_BUILD_TYPE Debug)
# add_compile_options(-std=c++14)
# set(CMAKE_CXX_FLAGS "-O0 -DDEBUG -std=c++14")
set(CMAKE_CXX_FLAGS "-std=c++14 -O3")
set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_FIND_DEBUG_MODE 1)
# set(CMAKE_CXX_FLAGS "-std=c++14")
# list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )

find_package(catkin REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Pangolin REQUIRED)
find_package(Boost REQUIRED)
# find_package(Sophus REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES Opencv
  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########
include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${OpenCV_INCLUDE_DIRS}
 ${EIGEN3_INCLUDE_DIR}
 ${GLEW_INCLUDE_DIRS}
 ${Pangolin_INCLUDE_DIRS}
 ${Boost_INCLUDE_DIRS}
#  ${Sophus_INCLUDE_DIRS}
 ${CSPARSE_INCLUDE_DIRS}
 ${G2O_INCLUDE_DIRS}
 ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/
)

set(g2o_LIBRARIES 
  g2o_core 
  g2o_types_sba 
  g2o_solver_csparse 
  g2o_stuff
  g2o_csparse_extension 
)

add_executable(Get_disparity src/Vision/Stereo/Get_disparity.cpp)
add_dependencies(Get_disparity ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Get_disparity
  # ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(plotTrajectory src/Vision/plotTrajectory.cpp)
add_dependencies(plotTrajectory ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(plotTrajectory
  # ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(calibrate src/Vision/Calibration/calibrate.cpp)
add_dependencies(calibrate ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(calibrate
  # ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

add_executable(undistort src/Vision/Calibration/undistort.cpp)
add_dependencies(undistort ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(undistort
  # ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)