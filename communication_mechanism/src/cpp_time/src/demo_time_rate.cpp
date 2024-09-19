#include "rclcpp/rclcpp.hpp"
using namespace std::chrono;
class Time : public rclcpp::Node
{
public:
  Time() : Node("Time_rate_node_cpp")
  {
    // 参数 刷新的频率
    rclcpp::Rate rate_1(2);
    // 参数 刷新的时间
    rclcpp::Rate rate_2(500ms);
    RCLCPP_INFO(this->get_logger(), "Time_rate 节点创建成功!");
    while (rclcpp::ok())
    {
      RCLCPP_INFO(this->get_logger(),"Hello ROS2! rate_1调用......");
      rate_1.sleep();
      RCLCPP_INFO(this->get_logger(),"Hello ROS2! rate_2调用......");
      rate_2.sleep();
    }
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