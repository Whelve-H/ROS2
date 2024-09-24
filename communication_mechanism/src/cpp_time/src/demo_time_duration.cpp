#include "rclcpp/rclcpp.hpp"
using namespace std::chrono;

class Time : public rclcpp::Node
{
public:
  Time() : Node("Time_duration_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Time_duration 节点创建成功!");
    // 创建Duration 对象
    rclcpp::Duration du_1(1s);
    rclcpp::Duration du_2(2,0.5e9);
    // 调用函数
    RCLCPP_INFO(this->get_logger(),"du_2: s= %.2f,ns= %ld",du_1.seconds(),du_1.nanoseconds());
    RCLCPP_INFO(this->get_logger(),"du_2: s= %.2f,ns= %ld",du_2.seconds(),du_2.nanoseconds());
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