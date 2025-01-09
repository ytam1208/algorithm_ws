#ifndef DUMMY_SUBSCRIBER_NODE_DUMMY_SUBSCRIBER_H_
#define DUMMY_SUBSCRIBER_NODE_DUMMY_SUBSCRIBER_H_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

namespace dummy_subscriber_node {

class DummySubscriber : public rclcpp::Node {
 public:
  explicit DummySubscriber(rclcpp::NodeOptions const& options);
  virtual ~DummySubscriber();

 private:
  void OnMessage(std_msgs::msg::String::SharedPtr const msg);
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
  size_t count_;
};

}
#endif  // DUMMY_SUBSCRIBER_NODE_DUMMY_SUBSCRIBER_H_
  