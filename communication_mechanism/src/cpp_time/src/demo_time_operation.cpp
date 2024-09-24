#include "rclcpp/rclcpp.hpp"
using namespace std::chrono;
class Time : public rclcpp::Node
{
public:
  Time() : Node("Time_operation_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Time_operation 节点创建成功!");
    // 时刻
    rclcpp::Time time_1(10, 0);
    rclcpp::Time time_2(20, 0);
    // 时间间隔
    rclcpp::Duration du_1(8, 0);
    rclcpp::Duration du_2(17, 0);

    // 比较运算
    RCLCPP_INFO(this->get_logger(), "time_1 >= time_2?(%s)", time_1 >= time_2 ? "Ture" : "False");
    // 数学运算
    rclcpp::Duration du_3 = time_1 - time_2;
    // 时刻之间的运算
    RCLCPP_INFO(this->get_logger(), "time_1-time_2=(%.2fs)", du_3.seconds());
    // 时间间隔之间的运算
    rclcpp::Duration du_4 = du_2 - du_1;
    RCLCPP_INFO(this->get_logger(), "du_4: s= %.2f,ns= %ld", du_4.seconds(), du_4.nanoseconds());
    // 时间间隔和时刻的运算
    rclcpp::Time time_3 = time_1 + du_1;
    RCLCPP_INFO(this->get_logger(), "time_3: s= %.2f,ns= %ld", time_3.seconds(), time_3.nanoseconds());
    //时刻之间的运算 不支持时刻之间的运算
    // rclcpp::Time time_4 =time_1+time_2;
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