#include "rclcpp/rclcpp.hpp"
#include "base_interface_demo/msg/student.hpp"

using namespace std::chrono_literals;

class Listener_define : public rclcpp::Node
{
public:
  Listener_define() : Node("listener_define_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "listener_define_node_cpp 节点创建成功!");
    subscription = this->create_subscription<base_interface_demo::msg::Student>("students",10,std::bind(&Listener_define::time_callback,this,std::placeholders::_1));
  }

private:
  void time_callback(const base_interface_demo::msg::Student &msg)
  {
    // std::cout << typeid(msg.age).name() << std::endl;
    RCLCPP_INFO(this->get_logger(),"订阅到的消息为:\n姓名:%s\n年龄:%d\n身高:%fm",msg.name.c_str(),msg.age,msg.height);
  }
  rclcpp::Subscription<base_interface_demo::msg::Student>::SharedPtr subscription;
  // subscription
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener_define >());
  return 0;
}