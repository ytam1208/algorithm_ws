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
SET( G2O_ROOT /usr/local/include/g2o )

find_package(catkin REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Pangolin REQUIRED)
find_package(Boost REQUIRED)
find_package(Sophus REQUIRED)

# find_package(G2O REQUIRED)
find_package(G2O REQUIRED)
# link_directories(${G2O_LIBRARIES})
# link_libraries(${G2O_LIBRARIES})


## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES Opencv
  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(${G2O_INCLUDE_DIRS})
# include_directories(/usr/local/include/g2o)
include_directories(
 include
 ${catkin_INCLUDE_DIRS}
 ${OpenCV_INCLUDE_DIRS}
 ${EIGEN3_INCLUDE_DIR}
 ${GLEW_INCLUDE_DIRS}
 ${Pangolin_INCLUDE_DIRS}
 ${Boost_INCLUDE_DIRS}
 ${Sophus_INCLUDE_DIRS}
 ${CSPARSE_INCLUDE_DIRS}
 ${G2O_INCLUDE_DIRS}
 ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/
)

# set( G2O_LIBRARIES 
#     g2o_core g2o_types_sba g2o_solver_csparse g2o_stuff g2o_csparse_extension 
# )
set( g2o_LIBRARIES 
  g2o_core g2o_types_sba g2o_solver_csparse g2o_stuff g2o_csparse_extension 
)
## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/Opencv.cpp
# )ToMat Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# 
## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(Optical_flow src/Vision/Optical_flow.cpp)
add_dependencies(Optical_flow ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Optical_flow
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(Stiching_image src/Vision/Stereo/Stiching_image.cpp)
add_dependencies(Stiching_image ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Stiching_image
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(Get_disparity src/Vision/Stereo/Get_disparity.cpp)
add_dependencies(Get_disparity ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Get_disparity
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(Get_epipolar src/Vision/Stereo/Get_epipolar.cpp)
add_dependencies(Get_epipolar ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Get_epipolar
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(${PROJECT_NAME} src/Vision/stereo_vision.cpp)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${Boost_LIBRARY}
  ${Pangolin_LIBRARIES}
)

add_executable(Sparse_direct_method src/Vision/RGB-D/Sparse_direct_method.cpp)
add_dependencies(Sparse_direct_method ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(Sparse_direct_method
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${Eigen3_LIBRARIES} 
  ${G2O_LIBRARIES}
  ${g2o_LIBRARIES}
)
#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_Opencv.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
# 