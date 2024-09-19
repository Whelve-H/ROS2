#include "rclcpp/rclcpp.hpp"

/*
需求: 以一个固定的频率发布文本消息"Hello World"，并且每次发布消息时,编号递增
    1.包含头文件
    2.初始化ROS节点
    3.自定义节点类
      3.1 创建发布者
      3.2 创建定时器
      3.3 组织并发布消息
    4.调用spin函数
    5.释放ROS资源
*/
#include "rclcpp/rclcpp.hpp"
class Param : public rclcpp::Node
{
public:
  Param() : Node("Param_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Param 节点创建成功!");

    rclcpp::Parameter p1("car_name", "Audi");
    rclcpp::Parameter p2("car_wheels", 4);
    rclcpp::Parameter p3("car_price", 12.3);
    // 获取值
    RCLCPP_INFO(this->get_logger(), "---------------获取值---------------");
    RCLCPP_INFO(this->get_logger(), "car_name:%s", p1.as_string().c_str());
    RCLCPP_INFO(this->get_logger(), "car_wheels:%ld", p2.as_int());
    RCLCPP_INFO(this->get_logger(), "car_price:%lf", p3.as_double());

    // 获取键
    RCLCPP_INFO(this->get_logger(), "---------------获取键--------------");
    RCLCPP_INFO(this->get_logger(), "p1的键值:   %s", p1.get_name().c_str());
    RCLCPP_INFO(this->get_logger(), "p2的键值:   %s", p2.get_name().c_str());
    RCLCPP_INFO(this->get_logger(), "p3的键值:   %s", p3.get_name().c_str());
    // 获取值的类型
    // RCLCPP_INFO(this->get_logger(), "---------------获取键值--------------");
    // RCLCPP_INFO(this->get_logger(), "p1的键值:   %s", p1.get_type().c_str());
    // RCLCPP_INFO(this->get_logger(), "p2的键值:   %s", p2.get_name().c_str());
    // RCLCPP_INFO(this->get_logger(), "p3的键值:   %s", p3.get_name().c_str());
    rclcpp::ParameterValue value(1);
    rclcpp::ParameterValue value_1(value);
    rclcpp::Parameter p4("demo",value);
    rclcpp::Parameter p5("demo_1",value_1);
    RCLCPP_INFO(this->get_logger(), "%s:%ld",p4.get_name().c_str(), p4.as_int());
    // RCLCPP_INFO(this->get_logger(), "%s:%s",p5.get_name().c_str(), p5.as_string().c_str());
    // RCLCPP_INFO(this->get_logger(), "car_wheels:%ld", p2.as_int());

  }
};

int main(int argc, char **argv)
{
  // 初始化节点
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Param>());
  rclcpp::shutdown();
  return 0;
}