#include "rclcpp/rclcpp.hpp"
using namespace std::chrono;
class Time : public rclcpp::Node
{
public:
  Time() : Node("Time_time_node_cpp")
  {
    // 创建Time 对象
    rclcpp::Time time_1(0.5e9L);    // ns  1s = 1e9ns
    rclcpp::Time time_2(2,0.5e9L); // s ns
    RCLCPP_INFO(this->get_logger(), "Time_time 节点创建成功!");
    rclcpp::Time right_now = this->now();
    // auto ra = time_1 -right_now;
    RCLCPP_INFO(this->get_logger(), "当前时间: s=%.2f,ns=%ld", time_1.seconds(), time_1.nanoseconds());
    RCLCPP_INFO(this->get_logger(), "当前时间: s=%.2f,ns=%ld", time_2.seconds(), time_2.nanoseconds());
    RCLCPP_INFO(this->get_logger(), "当前时间: s=%.2f,ns=%ld", right_now.seconds(), right_now.nanoseconds());
  }
};

int main(int argc, char **argv)
{
  // 初始化节点
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Time>());
  rclcpp::shutdown();
  return 0;
}