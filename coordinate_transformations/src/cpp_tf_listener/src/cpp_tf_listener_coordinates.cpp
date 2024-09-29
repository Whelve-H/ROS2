// 需求:  先发布laser到base_link的相对关系，在发布camera_link的相对关系
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
using namespace tf2_ros;
using namespace std::chrono_literals;
using geometry_msgs::msg::TransformStamped;
class cpp_tf_listener_coordinates : public rclcpp::Node
{
public:
    cpp_tf_listener_coordinates() : Node("cpp_tf_listener_coordinates_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "cpp_tf_listener_coordinates 节点创建成功!");
        // 3.1 创建缓存对象  融合多个坐标系相对的坐标关系为一颗坐标树
        /*
        TF2_ROS_PUBLIC Buffer(
        rclcpp::Clock::SharedPtr clock,
        tf2::Duration cache_time = tf2::Duration(tf2::BUFFER_CORE_DEFAULT_CACHE_TIME),
        rclcpp::Node::SharedPtr node = rclcpp::Node::SharedPtr());
        */
        buffer = std::make_unique<Buffer>(this->get_clock());
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
        listener = std::make_shared<TransformListener>(*buffer, this);
        // 3.3 编写一个定时器,循环实现转换
        timer = this->create_wall_timer(1s, std::bind(&cpp_tf_listener_coordinates::timer_callback, this));
    }

private:
    void timer_callback()
    {
        // geometry_msgs::msg::TransformStamped lookupTransform
        // (const std::string &target_frame,
        // const std::string &source_frame,
        // const tf2::TimePoint &time) const override;
        try
        {
            auto ts = buffer->lookupTransform("camera", "laser", tf2::TimePointZero);
            RCLCPP_INFO(this->get_logger(), "-----------转换完成的坐标帧消息-------------");
            RCLCPP_INFO(this->get_logger(), "父坐标系: %s", ts.header.frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "子坐标系: %s", ts.child_frame_id.c_str());
            RCLCPP_INFO(this->get_logger(), "偏移量为: (%lf,%lf,%lf)", ts.transform.translation.x, ts.transform.translation.y, ts.transform.translation.z);
        }
        //  * Possible exceptions tf2::LookupException, tf2::ConnectivityException,
        //  * tf2::ExtrapolationException, tf2::InvalidArgumentException
        catch (const tf2::LookupException &e)
        {
            RCLCPP_WARN(this->get_logger(), "异常提示: %s", e.what());
        }
    }
    std::unique_ptr<Buffer> buffer;
    std::shared_ptr<TransformListener> listener;
    rclcpp::TimerBase::SharedPtr timer;
};
int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<cpp_tf_listener_coordinates>());
    rclcpp::shutdown();
    return 0;
}