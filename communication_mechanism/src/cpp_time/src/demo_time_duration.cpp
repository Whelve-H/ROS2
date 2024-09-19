#include "rclcpp/rclcpp.hpp"
using namespace std::chrono;
class Time : public rclcpp::Node
{
public:
  Time() : Node("Time_duration_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Time_duration 节点创建成功!");
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