/*
    3.1 创建缓存对象  融合多个坐标系相对的坐标关系为一颗坐标树
    3.2 创建监听器,绑定缓存对象,会将所有的广播器广播写入缓存
    3.3 编写一个定时器,循环实现转换
*/
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "math.h"
#include "tf2/LinearMath/Quaternion.h"
using namespace std::chrono;
using geometry_msgs::msg::Twist;
class Exer_tf_listener : public rclcpp::Node
{
public:
    Exer_tf_listener(char **argv) : Node("Exer_tf_listener_cpp"), turtle_name_1(argv[1]), turtle_name_2(argv[2]), scaleRotationRate_(atof(argv[3]))
    {
        RCLCPP_INFO(this->get_logger(), "Exer_tf_listener 节点创建成功!");
        // 3.1 创建缓存对象  融合多个坐标系相对的坐标关系为一颗坐标树
        buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        // 3.2 创建监听器,绑定缓存对象,会将所有的广播器广播写入缓存
        /*
        TransformListener(
        tf2::BufferCore & buffer,
        NodeT && node,
        bool spin_thread = true,
        const rclcpp::QoS & qos = DynamicListenerQoS(),
        const rclcpp::QoS & static_qos = StaticListenerQoS(),
        const rclcpp::SubscriptionOptionsWithAllocator<AllocatorT> & options =
        detail::get_default_transform_listener_sub_options<AllocatorT>(),
        const rclcpp::SubscriptionOptionsWithAllocator<AllocatorT> & static_options =
        detail::get_default_transform_listener_static_sub_options<AllocatorT>())
        : buffer_(buffer)
        */
        transformListener_ = std::make_shared<tf2_ros::TransformListener>(*buffer_, this);
        // 3.3创建速度的发布方
        std::string pub_topic = "/" + turtle_name_2 + "/cmd_vel";
        pub_twist = this->create_publisher<Twist>(pub_topic, 10);
        // 3.4 编写一个定时器,循环实现转换,并且发布位置turtle1 的速度指令
        timer = this->create_wall_timer(100ms, std::bind(&Exer_tf_listener::timer_callback, this));
    }

private:
    void timer_callback()
    {
        // geometry_msgs::msg::TransformStamped ps = buffer_->lookupTransform(this->turtle_name_1,this->turtle_name_2,tf2::TimePointZero);
        try
        {
            auto ts = buffer_->lookupTransform(this->turtle_name_2, this->turtle_name_1, tf2::TimePointZero);
            RCLCPP_INFO(this->get_logger(), "-----------转换完成的坐标帧消息-------------");
            RCLCPP_INFO(this->get_logger(), "父坐标系: %s", ts.header.frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "子坐标系: %s", ts.child_frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "偏移量为: (%lf,%lf,%lf)", ts.transform.translation.x, ts.transform.translation.y, ts.transform.translation.z);
            auto twist = Twist();
            // 计算速度,并且发布速度指令
            static const double scaleForwardSpeed = 0.5;
            twist.linear.x = sqrt(pow(ts.transform.translation.x, 2) + pow(ts.transform.translation.y, 2)) * scaleForwardSpeed;
            twist.linear.y = 0;
            twist.linear.z = 0;
            // 四元数转换为欧拉角
            // 使用四元数转换为欧拉角
            tf2::Quaternion qua(
                ts.transform.rotation.x,
                ts.transform.rotation.y,
                ts.transform.rotation.z,
                ts.transform.rotation.w);
            double roll, pitch, yaw;
            tf2::Matrix3x3(qua).getRPY(roll, pitch, yaw); // 提取欧拉角
            RCLCPP_INFO(this->get_logger(), "当前欧拉角为: %lf", yaw);
            // 设置角速度
            static const double scaleRotationRate = this->scaleRotationRate_;
            twist.angular.x = 0;
            twist.angular.y = 0;
            // twist.angular.z = yaw * scaleRotationRate; // 将 yaw 作为角速度
            twist.angular.z = scaleRotationRate * atan2(
                                                      ts.transform.translation.y,
                                                      ts.transform.translation.x);
            pub_twist->publish(twist);
        }
        //  * Possible exceptions tf2::LookupException, tf2::ConnectivityException,
        //  * tf2::ExtrapolationException, tf2::InvalidArgumentException
        catch (const tf2::LookupException &e)
        {
            RCLCPP_WARN(this->get_logger(), "异常提示: %s", e.what());
        }
    }
    std::unique_ptr<tf2_ros::Buffer> buffer_;
    std::shared_ptr<tf2_ros::TransformListener> transformListener_;
    rclcpp::Publisher<Twist>::SharedPtr pub_twist;
    rclcpp::TimerBase::SharedPtr timer;
    std::string turtle_name_1;
    std::string turtle_name_2;
    double scaleRotationRate_;
};

int main(int argc, char **argv)
{
    // 初始化节点
    if (argc != 4)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请输入有效参数!!!(两只小乌龟的frame_id)");
    }
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer_tf_listener>(argv));
    rclcpp::shutdown();
    return 0;
}