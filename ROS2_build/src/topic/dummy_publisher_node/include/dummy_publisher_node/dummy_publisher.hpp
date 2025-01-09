#ifndef DUMMY_PUBLISHER_NODE_DUMMY_PUBLISHER_H_
#define DUMMY_PUBLISHER_NODE_DUMMY_PUBLISHER_H_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

namespace dummy_publisher_node {

class DummyPublisher : public rclcpp::Node {
 public:
  explicit DummyPublisher(const rclcpp::NodeOptions& options);
  
 private:
  void OnTimer();

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std_msgs::msg::String message_;
  size_t count_;
};

}
#endif  // DUMMY_PUBLISHER_NODE_DUMMY_PUBLISHER_H_