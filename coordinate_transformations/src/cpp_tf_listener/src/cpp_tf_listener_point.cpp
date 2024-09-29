// // #include "rclcpp/rclcpp.hpp"
// // #include "geometry_msgs/msg/transform_stamped.hpp"
// // #include "tf2_ros/buffer.h"
// // #include "tf2_ros/transform_listener.h"
// // #include "geometry_msgs/msg/point_stamped.hpp"
// // #include "tf2_ros/create_timer_ros.h"
// // // 过滤器
// // #include "tf2_ros/message_filter.h"
// // #include "tf2_geometry_msgs/tf2_geometry_msgs/tf2_geometry_msgs.hpp"
// // // point 消息订阅方
// // #include "message_filters/subscriber.h"
// #include <geometry_msgs/msg/point_stamped.hpp>
// #include <message_filters/subscriber.h>

// #include <rclcpp/rclcpp.hpp>
// #include <tf2_ros/buffer.h>
// #include <tf2_ros/create_timer_ros.h>
// #include <tf2_ros/message_filter.h>
// #include <tf2_ros/transform_listener.h>
// // #ifdef TF2_CPP_HEADERS
// //   #include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
// // #else
// //   #include <tf2_geometry_msgs/tf2_geometry_msgs.h>
// // #endif


// using namespace geometry_msgs::msg;
// using namespace std::chrono_literals;
// class cpp_tf_listener_point : public rclcpp::Node
// {
// public:
//     cpp_tf_listener_point() : Node("cpp_tf_listener_point_cpp")
//     {
//         RCLCPP_INFO(this->get_logger(), "cpp_tf_listener_point 节点创建成功!");
//         // 3.1 创建缓存对象  融合多个坐标系相对的坐标关系为一颗坐标树
//         buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());
//         // CreateTimerROS
//         // (rclcpp::node_interfaces::NodeBaseInterface::SharedPtr node_base,
//         // rclcpp::node_interfaces::NodeTimersInterface::SharedPtr node_timers,
//         // rclcpp::CallbackGroup::SharedPtr callback_group = nullptr)
//         timer = std::make_shared<tf2_ros::CreateTimerROS>(this->get_node_base_interface(), this->get_node_timers_interface());
//         // void setCreateTimerInterface(tf2_ros::CreateTimerInterface::SharedPtr create_timer_interface)
//         buffer->setCreateTimerInterface(timer);
//         // 3.2 创建监听器,绑定缓存对象,会将所有的广播器广播写入缓存
//         listener = std::make_shared<tf2_ros::TransformListener>(*buffer, this);
//         // 3.3 创建一个点数据的订阅方
//         // Subscriber(rclcpp::Node *node, const std::string &topic, const rmw_qos_profile_t qos)
//         sub.subscribe(this, "point");
//         // 3.4 创建一个过滤器，输入一个坐标树和坐标点,然后求解数据
//         /*
//         MessageFilter(
//         F & f,                                                                           数据订阅方
//         BufferT & buffer,                                                                消息缓存
//         const std::string & target_frame,                                                父级坐标系
//         uint32_t queue_size,                                                             队列大小
//         const rclcpp::node_interfaces::NodeLoggingInterface::SharedPtr & node_logging,   日志接口
//         const rclcpp::node_interfaces::NodeClockInterface::SharedPtr & node_clock,       时钟接口
//         std::chrono::duration<TimeRepT, TimeT> buffer_timeout =                          超时时间
//         std::chrono::duration<TimeRepT, TimeT>::max())
//          : node_logging_(node_logging),
//         node_clock_(node_clock),
//         buffer_(buffer),
//         queue_size_(queue_size),
//         buffer_timeout_(buffer_timeout)
//             {
//                 init();
//                 setTargetFrame(target_frame);
//                 connectInput(f);
//             }
//         */
//         filter = std::make_shared<tf2_ros::MessageFilter<PointStamped>>(
//             sub,
//             *buffer,
//             "base_link",
//             10,
//             this->get_node_logging_interface(),
//             this->get_node_clock_interface(),
//             1s);
//         // 数据解析
//         filter->registerCallback(&cpp_tf_listener_point::filter_callback, this);
//     }

// private:
//     void filter_callback(const PointStamped &ps)
//     {
//         // 实现坐标点的变换
//         // T transform<T>(const T &in, const std::string &target_frame, tf2::Duration timeout = tf2::durationFromSec((0.0))) const
//         try
//         {
//             auto out = buffer->transform(ps, "base_link");
//             RCLCPP_INFO(this->get_logger(), "-----------转换完成的坐标帧消息-------------");
//             RCLCPP_INFO(this->get_logger(), "父坐标系: %s", out.header.frame_id.c_str());
//             RCLCPP_INFO(this->get_logger(), "偏移量为: (%lf,%lf,%lf)", out.point.x, out.point.y, out.point.z);
//         }
//         catch (const std::exception &e)
//         {
//             RCLCPP_WARN(this->get_logger(), "WARNING...........");
//         }
//     }
//     std::unique_ptr<tf2_ros::Buffer> buffer;
//     std::shared_ptr<tf2_ros::TransformListener> listener;
//     std::shared_ptr<tf2_ros::CreateTimerROS> timer;
//     std::shared_ptr<tf2_ros::MessageFilter<PointStamped>> filter;
//     message_filters::Subscriber<PointStamped> sub;
// };

// int main(int argc, char **argv)
// {
//     // 初始化节点
//     rclcpp::init(argc, argv);
//     rclcpp::spin(std::make_shared<cpp_tf_listener_point>());
//     rclcpp::shutdown();
//     return 0;
// }


#include <geometry_msgs/msg/point_stamped.hpp>
#include <message_filters/subscriber.h>
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/buffer.h>
#include <tf2_ros/create_timer_ros.h>
#include <tf2_ros/message_filter.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp> // 确保包含这个头文件

using namespace geometry_msgs::msg;
using namespace std::chrono_literals;

class cpp_tf_listener_point : public rclcpp::Node
{
public:
    cpp_tf_listener_point() : Node("cpp_tf_listener_point_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "cpp_tf_listener_point 节点创建成功!");

        buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        timer = std::make_shared<tf2_ros::CreateTimerROS>(this->get_node_base_interface(), this->get_node_timers_interface());
        buffer->setCreateTimerInterface(timer);

        listener = std::make_shared<tf2_ros::TransformListener>(*buffer, this);
        sub.subscribe(this, "point");

        filter = std::make_shared<tf2_ros::MessageFilter<PointStamped>>(
            sub,
            *buffer,
            "base_link",
            10,
            this->get_node_logging_interface(),
            this->get_node_clock_interface(),
            1s
        );

        filter->registerCallback(&cpp_tf_listener_point::filter_callback, this);
    }

private:
    void filter_callback(const PointStamped &ps)
    {
        try
        {
            auto out = buffer->transform(ps, "base_link");
            RCLCPP_INFO(this->get_logger(), "-----------转换完成的坐标帧消息-------------");
            RCLCPP_INFO(this->get_logger(), "父坐标系: %s", out.header.frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "偏移量为: (%lf,%lf,%lf)", out.point.x, out.point.y, out.point.z);
        }
        catch (const std::exception &e)
        {
            RCLCPP_WARN(this->get_logger(), "WARNING: %s", e.what());
        }
    }

    std::unique_ptr<tf2_ros::Buffer> buffer;
    std::shared_ptr<tf2_ros::TransformListener> listener;
    std::shared_ptr<tf2_ros::CreateTimerROS> timer;
    std::shared_ptr<tf2_ros::MessageFilter<PointStamped>> filter;
    message_filters::Subscriber<PointStamped> sub;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<cpp_tf_listener_point>());
    rclcpp::shutdown();
    return 0;
}
