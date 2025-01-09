# Build 
colcon build --packages-select dummy_publisher
. install/setup.bash

# Run container
ros2 run rclcpp_components component_container

# Load component
ros2 component load /ComponentManager dummy_publisher dummy_publisher::DummyPublisherComponent