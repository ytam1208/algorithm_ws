cmake_minimum_required(VERSION 3.0.2)
project(rospub)

#Debug mode
set(CMAKE_VERBOSE_MAKEFILE yes)
set(CMAKE_BUILD_TYPE Debug)

#Release mode
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "-O0 -DDEBUG -std=c++11")

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME} 
  CATKIN_DEPENDS roscpp std_msgs
#  DEPENDS system_lib
)

include_directories(
 include
  ${catkin_INCLUDE_DIRS}
)

 add_executable(${PROJECT_NAME} src/main.cpp)
 add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

 target_link_libraries(${PROJECT_NAME}
   ${catkin_LIBRARIES}
 )
