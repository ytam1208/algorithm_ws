#ifndef _DUMMY_PUBLISHER_DUMMY_PUBLISHER_COMPONENT_H_
#define _DUMMY_PUBLISHER_DUMMY_PUBLISHER_COMPONENT_H_

#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

namespace dummy_publisher {
/**
 * @brief A dummy publisher component that demonstrates ROS2 publisher functionality.
 *
 * This component publishes string messages to a 'talker' topic at regular intervals.
 */
class DummyPublisherComponent : public rclcpp::Node {
 public:
  /**
   * @brief Constructs a new DummyPublisherComponent.
   * @param options The node options for ROS2 node configuration.
   */
  explicit DummyPublisherComponent(const rclcpp::NodeOptions& options);
  virtual ~DummyPublisherComponent();

 private:
  /**
   * @brief Timer callback function that publishes messages.
   */
  void OnTimer();

  // Order members by size (largest to smallest) to minimize padding
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;  // 8 bytes
  rclcpp::TimerBase::SharedPtr timer_;                            // 8 bytes
  std_msgs::msg::String message_;                                 // varies
  size_t count_;                                                  // 8 bytes
};

}  // namespace dummy_publisher

#endif  // DUMMY_PUBLISHER_DUMMY_PUBLISHER_COMPONENT_H_