#include "dummy_subscriber_node/dummy_subscriber.hpp"

namespace dummy_subscriber_node {

namespace {
  static std::string const kTopicName = "help_me";
  static int const kQueueSize = 1;
}

DummySubscriber::DummySubscriber(rclcpp::NodeOptions const& options)
  : Node("dummy_subscriber", options) {
    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        kTopicName, kQueueSize, 
        std::bind(&DummySubscriber::OnMessage, this, std::placeholders::_1));
  }

DummySubscriber::~DummySubscriber() {
  RCLCPP_INFO(this->get_logger(), "Destroying DummySubscriber");
  subscriber_.reset();
}

void DummySubscriber::OnMessage(std_msgs::msg::String::SharedPtr const msg) {
  RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data.c_str());
}

}

int main(int argc, char ** argv) {
  rclcpp::init(argc, argv);
  
  rclcpp::NodeOptions options;
  options.arguments({"--ros-args", "--log-level", "info"});
  
  rclcpp::spin(
      std::make_shared<dummy_subscriber_node::DummySubscriber>(options));
  rclcpp::shutdown();
  return 0;
}
