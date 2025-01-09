#ifndef DUMMY_SUBSCRIBER_COMPONENT_DUMMY_SUBSCRIBER_COMPONENT_H_
#define DUMMY_SUBSCRIBER_COMPONENT_DUMMY_SUBSCRIBER_COMPONENT_H_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

namespace dummy_subscriber_component {

class DummySubscriberComponent : public rclcpp::Node {
 public:
  explicit DummySubscriberComponent(const rclcpp::NodeOptions& options);
  virtual ~DummySubscriberComponent();
  
 private:
  void OnMessage(const std_msgs::msg::String::SharedPtr msg);

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
  size_t count_;
};

}  // namespace dummy_subscriber_component

#endif  // DUMMY_SUBSCRIBER_COMPONENT_DUMMY_SUBSCRIBER_COMPONENT_H_