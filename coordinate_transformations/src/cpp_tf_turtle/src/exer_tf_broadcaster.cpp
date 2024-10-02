/*
        // 3.1  创建一个动态广播器
        // 3.2  创建一个乌龟的位姿订阅方
        // 3.3  回调函数中,获取乌龟的位置信息并且生成相对的关系然后发布
*/
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
// 动态广播器
#include "tf2_ros/transform_broadcaster.h"
// 四元数和RPY转换
#include "tf2/LinearMath/Quaternion.h"
using geometry_msgs::msg::Pose;
using geometry_msgs::msg::TransformStamped;
class Exer_tf_broadcater : public rclcpp::Node
{
public:
    Exer_tf_broadcater() : Node("Exer_tf_broadcater_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Exer_tf_broadcater 节点创建成功!");
        // 3.1  创建一个动态广播器
        this->transformBroadcaster = std::make_shared<tf2_ros::TransformBroadcaster>();
        // 3.2  创建一个乌龟的位姿订阅方
        std::string topic_name = "";
        this->create_subscription<Pose>(topic_name, 10, std::bind(&Exer_tf_broadcater::sub_callback, this,std::placeholders::_1));
        // 3.3  回调函数中,获取乌龟的位置信息并且生成相对的关系然后发布
    }

private:
    void sub_callback(Pose &pose)
    {
        TransformStamped transformStamped;
        transformStamped.child_frame_id
        // void sendTransform(const geometry_msgs::msg::TransformStamped &transform)
        this->transformBroadcaster->sendTransform(transformStamped);
    }
    rclcpp::Subscription<Pose>::SharedPtr sub;
    std::shared_ptr<tf2_ros::TransformBroadcaster> transformBroadcaster;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer_tf_broadcater>());
    rclcpp::shutdown();
    return 0;
}