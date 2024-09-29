#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
using geometry_msgs::msg::TransformStamped;
using tf2::Quaternion;
using tf2_ros::StaticTransformBroadcaster;
class cpp_static_tf_broadcaster : public rclcpp::Node
{
public:
    cpp_static_tf_broadcaster(char** argv) : Node("cpp_static_tf_broadcaster_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "cpp_static_tf_broadcaster 节点创建成功!");
        // 创建广播对象
        static_tf_broadcaster = std::make_shared<StaticTransformBroadcaster>(this);
                                // 组织并且发布数据
        this->pub_static_tf(argv);
    }

private:
    void pub_static_tf(char **argv)
    {
        // 组织消息并发送
        geometry_msgs::msg::TransformStamped transform;
        // header  时间戳
        transform.header.stamp = this->now();
        // 父极坐标系
        transform.header.frame_id = argv[7];
        // 子集坐标系
        transform.child_frame_id = argv[8];
        // 坐标偏移量
        transform.transform.translation.x = atoi(argv[1]);
        transform.transform.translation.y = atoi(argv[2]);
        transform.transform.translation.z = atoi(argv[3]);
        // 欧拉角转换为四元数
        Quaternion qua;
        // typedef double tf2Scalar;
        // void setRPY(const tf2Scalar &roll, const tf2Scalar &pitch, const tf2Scalar &yaw)
        qua.setRPY(atof(argv[4]), atof(argv[5]), atof(argv[6]));
        // 四元数
        transform.transform.rotation.x = qua.getX();
        transform.transform.rotation.y = qua.getY();
        transform.transform.rotation.z = qua.getZ();
        transform.transform.rotation.w = qua.getW();
        // void sendTransform(const geometry_msgs::msg::TransformStamped &transform)
        // void sendTransform(const std::vector<geometry_msgs::msg::TransformStamped, std::allocator<geometry_msgs::msg::TransformStamped>> &transforms)
        this->static_tf_broadcaster->sendTransform(transform);
    }
    std::shared_ptr<StaticTransformBroadcaster> static_tf_broadcaster;
};

int main(int argc, char **argv)
{
    // 判断传入的数据是否违法
    if (argc != 9)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"参数有误!!!");
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"x y z roll pitch yaw header_frame_id child_frame_id");
    }
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<cpp_static_tf_broadcaster>(argv));
    rclcpp::shutdown();
    return 0;
}