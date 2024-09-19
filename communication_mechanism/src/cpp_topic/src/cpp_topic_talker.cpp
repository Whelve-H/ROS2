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
#include "std_msgs/msg/string.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

class Talker : public rclcpp::Node
{
public:
  Talker() : Node("talker_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "talker_node_cpp 节点创建");
    // 参数2: Qos 消息队列
    publishers = this->create_publisher<std_msgs::msg::String>("chatter", 10);
    // rclcpp::WallTimer<CallbackT, nullptr>::SharedPtr create_wall_timer<DurationRepT, DurationT, CallbackT>(std::chrono::duration<DurationRepT, DurationT> period, CallbackT callback, rclcpp::CallbackGroup::SharedPtr group = nullptr)
    time_ = this->create_wall_timer(100ms, std::bind(&Talker::on_timer, this));
  }

private:
  void on_timer(){
    // 回调函数组织并且发布消息
    // void publish(const rclcpp::SerializedMessage &serialized_msg)
    auto  msg = std_msgs::msg::String();
    msg.data  = std::to_string(count++)+".  "+"hello world";
    RCLCPP_INFO(this->get_logger(),"发布方发布的消息为:%s",msg.data.c_str());
    publishers->publish(msg);
  }
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publishers;
  rclcpp::TimerBase::SharedPtr time_;
  size_t count = 0 ;
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Talker>());
  // 资源的释放
  rclcpp::shutdown();
  return 0;
}