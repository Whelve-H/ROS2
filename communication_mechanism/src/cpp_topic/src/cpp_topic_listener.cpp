#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class Listener : public rclcpp::Node
{
public:
  Listener() : Node("listener_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "listener_node_cpp 节点创建成功!");
    subscription = this->create_subscription<std_msgs::msg::String>("chatter",10,std::bind(&Listener::callback,this,std::placeholders::_1));
  }

private:
  void callback(const std_msgs::msg::String &msg)
  {
    RCLCPP_INFO(this->get_logger(),"订阅到的消息为: %s",msg.data.c_str());
  }
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription;
  // subscription
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Listener>());
  return 0;
}