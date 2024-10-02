/*
        // 3.1  创建一个动态广播器
        // 3.2  创建一个乌龟的位姿订阅方
        // 3.3  回调函数中,获取乌龟的位置信息并且生成相对的关系然后发布
*/
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
// 动态广播器
#include "tf2_ros/transform_broadcaster.h"
// 四元数和RPY转换
#include "tf2/LinearMath/Quaternion.h"
using turtlesim::msg::Pose;
using geometry_msgs::msg::TransformStamped;
using tf2::Quaternion;
class Exer_tf_broadcater : public rclcpp::Node
{
public:
    Exer_tf_broadcater(const std::string &name_) : Node("Exer_tf_broadcater_cpp"),name(name_)
    {   
        RCLCPP_INFO(this->get_logger(), "Exer_tf_broadcater 节点创建成功!");
        // 3.1  创建一个动态广播器
        this->transformBroadcaster = std::make_shared<tf2_ros::TransformBroadcaster>(this);
        // 3.2  创建一个乌龟的位姿订阅方
        std::string topic_name =  "/" + name + "/pose";
        RCLCPP_INFO(this->get_logger(),"当前的订阅话题为: %s",topic_name.c_str());
        sub = this->create_subscription<Pose>(topic_name, 10, std::bind(&Exer_tf_broadcater::sub_callback, this,std::placeholders::_1));
        // 3.3  回调函数中,获取乌龟的位置信息并且生成相对的关系然后发布
    }
private:
    void sub_callback(const Pose &pose)
    {
        
        // RCLCPP_INFO(this->get_logger(),"当前的乌龟的坐标为(%f,%f,%f)",pose.x,pose.y,pose.theta);
        TransformStamped trans;
        Quaternion qua;
        trans.header.stamp = this->now();
        trans.header.frame_id = "world";

        trans.child_frame_id = name;
        
        trans.transform.translation.x = pose.x;
        trans.transform.translation.y = pose.y;
        trans.transform.translation.z = 0.0;
        
        qua.setRPY(0.0,0.0,pose.theta);
        trans.transform.rotation.x = qua.getX();
        trans.transform.rotation.y = qua.getY();
        trans.transform.rotation.z = qua.getZ();
        trans.transform.rotation.w = qua.getW();
        // void sendTransform(const geometry_msgs::msg::TransformStamped &transform)
        this->transformBroadcaster->sendTransform(trans);
    }
    rclcpp::Subscription<Pose>::SharedPtr sub;
    std::shared_ptr<tf2_ros::TransformBroadcaster> transformBroadcaster;
    std::string name;
};

int main(int argc, char **argv)
{
    // 初始化节点
    if(argc != 2)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"参数输入有误!!!");
        return 0;
    }
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer_tf_broadcater>(argv[1]));
    rclcpp::shutdown();
    return 0;
}