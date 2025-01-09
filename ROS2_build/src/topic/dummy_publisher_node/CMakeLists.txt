cmake_minimum_required(VERSION 3.8)
project(dummy_publisher_node)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

################################
#      Find Dependencies       #
################################
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

################################
#    Create the executable     #
################################
add_executable(dummy_publisher_node src/dummy_publisher_node.cpp)
################################
#     Include Directories      #
################################
target_include_directories(dummy_publisher_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
################################
#       ROS2 Dependencies      #
################################
ament_target_dependencies(dummy_publisher_node
  rclcpp
  std_msgs
)
################################
#      Compile Features        #
################################
target_compile_features(dummy_publisher_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
################################
#       Install targets        #
################################
install(TARGETS dummy_publisher_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
