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
#include "base_interface_demo/msg/student.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class Talker_define : public rclcpp::Node
{
public:
  Talker_define() : Node("talker_define_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "talker_define_node_cpp 节点创建");
    // 参数2: Qos 消息队列
    publishers = this->create_publisher<base_interface_demo::msg::Student>("students", 10);
    // // rclcpp::WallTimer<CallbackT, nullptr>::SharedPtr create_wall_timer<DurationRepT, DurationT, CallbackT>(std::chrono::duration<DurationRepT, DurationT> period, CallbackT callback, rclcpp::CallbackGroup::SharedPtr group = nullptr)
    time_ = this->create_wall_timer(100ms, std::bind(&Talker_define::time_callback, this));
  }

private:
  void time_callback()
  {
    // 回调函数组织并且发布消息
    // void publish(const rclcpp::SerializedMessage &serialized_msg)
    auto msg = base_interface_demo::msg::Student();
    msg.age = 12;
    msg.name = "小黄同学";
    msg.height = 1.71;
    RCLCPP_INFO(this->get_logger(),"学生的姓名:%s \n年龄:%d\n身高为:%fm", msg.name.c_str(), msg.age, msg.height);
    publishers->publish(msg);
  }
  rclcpp::Publisher<base_interface_demo::msg::Student>::SharedPtr publishers;
  rclcpp::TimerBase::SharedPtr time_;
  size_t count = 0;
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Talker_define>());
  // 资源的释放
  rclcpp::shutdown();
  return 0;
}