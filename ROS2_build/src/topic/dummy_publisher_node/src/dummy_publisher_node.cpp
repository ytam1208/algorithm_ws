#include "dummy_publisher_node/dummy_publisher.hpp"

namespace dummy_publisher_node {
  
  DummyPublisher::DummyPublisher(const rclcpp::NodeOptions& options)
    : Node("dummy_publisher", options)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("dummy_publisher", 10);
    timer_ = this->create_wall_timer(
        std::chrono::seconds(1), 
        std::bind(&DummyPublisher::OnTimer, this));
  }
  
  void DummyPublisher::OnTimer()
  {
    message_.data = "Hello, world! " + std::to_string(count_++);
    publisher_->publish(message_);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message_.data.c_str());
  }

}

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  rclcpp::NodeOptions options;
  options.arguments({"--ros-args", "--log-level", "info"});
  
  rclcpp::spin(std::make_shared<dummy_publisher_node::DummyPublisher>(options));
  rclcpp::shutdown();
  return 0;
}
