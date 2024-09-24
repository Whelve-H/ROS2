#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/twist.hpp"

using geometry_msgs::msg::Twist;
using turtlesim::msg::Pose;
using namespace std::chrono;
class Exe_pub_sub : public rclcpp::Node
{
public:
    Exe_pub_sub() : Node("Pose_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Exe_pub_sub 节点创建成功!");
        pose_subscription = this->create_subscription<Pose>("/turtle1/pose", 10, std::bind(&Exe_pub_sub::pose_callbackGroup, this, std::placeholders::_1));
        // std::shared_ptr<PublisherT> create_publisher<MessageT, AllocatorT, PublisherT>
        // (const std::string &topic_name,
        // const rclcpp::QoS &qos,
        // const rclcpp::PublisherOptionsWithAllocator<AllocatorT> &options = rclcpp::PublisherOptionsWithAllocator<AllocatorT>())
        cmd_vel_publisher = this->create_publisher<Twist>("/t2/turtle1/cmd_vel", 20);
        // rclcpp::WallTimer<CallbackT, nullptr>::SharedPtr create_wall_timer<DurationRepT, DurationT, CallbackT>
        // (std::chrono::duration<DurationRepT, DurationT> period, CallbackT callback, rclcpp::CallbackGroup::SharedPtr group = nullptr)
    }

private:
    void pose_callbackGroup(const Pose &msg)
    {
        Twist twist;
        // 角速度取反,线速度不变
        twist.angular.z = -(msg.angular_velocity);
        twist.linear.x = msg.linear_velocity;
        cmd_vel_publisher ->publish(twist);
    }
    rclcpp::Subscription<Pose>::SharedPtr pose_subscription;
    rclcpp::Publisher<Twist>::SharedPtr cmd_vel_publisher;
    rclcpp::TimerBase::SharedPtr time_;
    Pose pose;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exe_pub_sub>());
    rclcpp::shutdown();
    return 0;
}