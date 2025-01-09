#include "dummy_subscriber_component/dummy_subscriber_component.hpp"

namespace dummy_subscriber_component {

namespace {
  const std::string kTopicName = "help_me";
  const int kQueueSize = 1;  
}

  DummySubscriberComponent::DummySubscriberComponent(
      const rclcpp::NodeOptions& options)
    : Node("dummy_subscriber_component", options) {
    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        kTopicName, kQueueSize, 
        std::bind(&DummySubscriberComponent::OnMessage, this, std::placeholders::_1));
  }

  DummySubscriberComponent::~DummySubscriberComponent() {
    RCLCPP_INFO(this->get_logger(), "Destroying DummySubscriberComponent");
    subscriber_.reset();
  }

  void DummySubscriberComponent::OnMessage(
      const std_msgs::msg::String::SharedPtr msg) {
    RCLCPP_INFO(this->get_logger(), 
        "Received message: '%s'", msg->data.c_str());
  }
}

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(
    dummy_subscriber_component::DummySubscriberComponent)