cmake_minimum_required(VERSION 3.8)
project(dummy_publisher_component)

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
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)

################################
# Create the component library #
################################
add_library(dummy_publisher SHARED
  src/dummy_publisher_component.cpp
)
################################
#     Include Directories      #
################################
target_include_directories(dummy_publisher
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
################################
#       Link Libraries         #
################################
# target_link_libraries(dummy_publisher
#   OpenCV::opencv_core
# )
################################
#     ROS2 Dependencies        #
################################
ament_target_dependencies(dummy_publisher
  rclcpp 
  rclcpp_components 
  std_msgs
)
################################
#    Create Register Nodes     #
################################
rclcpp_components_register_nodes(dummy_publisher 
  PLUGIN "dummy_publisher::DummyPublisherComponent"
  EXECUTABLE "dummy_publisher_component_node"
)
################################
#       Install targets        #
################################
install(TARGETS dummy_publisher
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY include/
  DESTINATION include
)
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}
)

# ament_export_dependencies(rclcpp rclcpp_components std_msgs)
# ament_export_targets(export_dummy_publisher)
ament_package()
