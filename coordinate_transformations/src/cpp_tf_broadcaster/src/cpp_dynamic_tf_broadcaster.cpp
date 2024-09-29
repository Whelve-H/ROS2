#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "turtlesim/msg/pose.hpp"
#include "tf2/LinearMath/Quaternion.h"

using geometry_msgs::msg::TransformStamped;
using turtlesim::msg::Pose;
using tf2::Quaternion;

class Cpp_dynamic_tf_broadcaster : public rclcpp::Node
{
public:
    Cpp_dynamic_tf_broadcaster() : Node("Cpp_dynamic_tf_broadcaster_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Cpp_dynamic_tf_broadcaster 节点创建成功!");
        // 3.1  创建一个动态广播器
        dynamic_tf2_broadcaster = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        // 3.2  创建一个乌龟的位姿订阅方
        sub = this->create_subscription<Pose>("/turtle1/pose",10,std::bind(&Cpp_dynamic_tf_broadcaster::sub_callback,this,std::placeholders::_1));
        // 3.3  回调函数中,获取乌龟的位置信息并且生成相对的关系然后发布
    }
private:
void sub_callback(const Pose &pose)
{
    // 组织消息
    TransformStamped trans;
    trans.header.stamp = this->now();
    trans.header.frame_id = "world";
    
    trans.child_frame_id = "turtle1";
    
    trans.transform.translation.x = pose.x;
    trans.transform.translation.y = pose.y;
    trans.transform.translation.z = 0.0;

    // 转换
    Quaternion qua;
    // 只有 yaw 没有pitch 和 roll
    qua.setRPY(0.0,0.0,pose.theta);
    
    trans.transform.rotation.x = qua.getX();
    trans.transform.rotation.y = qua.getY();
    trans.transform.rotation.z = qua.getZ();
    trans.transform.rotation.w = qua.getW();

    // 发布
    this->dynamic_tf2_broadcaster->sendTransform(trans);
}
std::shared_ptr<tf2_ros::TransformBroadcaster> dynamic_tf2_broadcaster;
rclcpp::Subscription<Pose>::SharedPtr sub;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Cpp_dynamic_tf_broadcaster>());
    rclcpp::shutdown();
    return 0;
}