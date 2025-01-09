#include "dummy_publisher/dummy_publisher_component.hpp"

namespace dummy_publisher {

namespace {
const std::string kTopicName = "help_me";
const std::string kDefaultMessage = "Hello, ROS2!";
constexpr int kQueueSize = 1;
constexpr auto kPublishPeriod = std::chrono::seconds(1);
}  // namespace

DummyPublisherComponent::DummyPublisherComponent(
    const rclcpp::NodeOptions& options)
    : Node("dummy_publisher", options),
      count_(0) {
  publisher_ = create_publisher<std_msgs::msg::String>(kTopicName, kQueueSize);
  message_.data = kDefaultMessage;
  
  timer_ = create_wall_timer(
      kPublishPeriod,
      std::bind(&DummyPublisherComponent::OnTimer, this));
}

DummyPublisherComponent::~DummyPublisherComponent() {
  RCLCPP_INFO(this->get_logger(), "Destroying DummyPublisherComponent");
  timer_->cancel();
  timer_.reset();
  publisher_.reset();
}

void DummyPublisherComponent::OnTimer() {
  message_.data = kDefaultMessage + std::to_string(count_++);
  RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message_.data.c_str());
  publisher_->publish(message_);
}

}  // namespace dummy_publisher

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(dummy_publisher::DummyPublisherComponent)