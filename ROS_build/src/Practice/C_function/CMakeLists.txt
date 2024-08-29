cmake_minimum_required(VERSION 3.0.2)
project(C_function)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)
set(CMAKE_BUILD_TYPE Release)
add_compile_options(-std=c++14)

find_package(catkin REQUIRED COMPONENTS
  roscpp
)
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
#find_package(Boost REQUIRED COMPONENTS thread)

catkin_package(
  INCLUDE_DIRS include
  #LIBRARIES ${PROJECT_NAME}
  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${Boost_INCLUDE_DIRS}
 ${OpenCV_INCLUDE_DIRS}
 ${PROJECT_SOURCE_DIR}/include/
)

add_executable(${PROJECT_NAME} src/Modern_Cplusplus/of.cpp)

add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}
   ${catkin_LIBRARIES}
   ${OpenCV_LIBRARIES}
  -lpthread
)