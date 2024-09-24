#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

using turtlesim::msg::Pose;
class Pose_topic : public rclcpp::Node
{
public:
    Pose_topic() : Node("Pose_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Pose_topic 节点创建成功!");
        // const std::string &service_name, const rmw_qos_profile_t &qos_profile = rmw_qos_profile_services_default, rclcpp::CallbackGroup::SharedPtr group = nullptr
        subscription = this->create_subscription<Pose>("/turtle1/pose",10,std::bind(&Pose_topic::CallbackGroup,this,std::placeholders::_1));
        // subscription = this->create_subscription<Pose>("chatter",10,std::bind(&Pose_topic::callbackGroup,this,std::placeholders::_1));
    }
private:
void CallbackGroup(const Pose &msg)
{
    RCLCPP_INFO(this->get_logger(),"x:%f",msg.x);
    RCLCPP_INFO(this->get_logger(),"y:%f",msg.y);
    RCLCPP_INFO(this->get_logger(),"theta:%f",msg.theta);
    RCLCPP_INFO(this->get_logger(),"linear_velocity:%f",msg.linear_velocity);
    RCLCPP_INFO(this->get_logger(),"angular_velocity:%f",msg.angular_velocity);
    RCLCPP_INFO(this->get_logger(),"------------------------------------------");
}
rclcpp::Subscription<Pose>::SharedPtr subscription;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Pose_topic>());
    rclcpp::shutdown();
    return 0;
}