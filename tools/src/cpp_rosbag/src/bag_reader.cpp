/*
    需求: 录制控制乌龟的速度指令
    流程:
        1. 包含头文件
        2. 初始化ROS2的客户端
        3. 自定义节点
            3-1. 创建一个回放的对象
            3-2. 设置被读取的文件
            3-3. 读取消息
            3-4. 关闭文件
        4. 调用spain函数,并且传入节点对象指针
        5.资源的释放
/home/ros/ROS2/communication_mechanism
*/
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rosbag2_cpp/reader.hpp"
using geometry_msgs::msg::Twist;
using rosbag2_cpp::Reader;
class Bag_reader : public rclcpp::Node
{
public:
    Bag_reader() : Node("Bag_reader_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Bag_reader 节点创建成功!");
        // 3-1. 创建一个回放的对象
        reader = std::make_unique<Reader>();
        // 3-2. 设置被读取的文件
        reader->open("my_bag");
        // 3-3. 读取消息
        while(reader->has_next())
        {
            auto twist = reader->read_next<Twist>();
            RCLCPP_INFO(this->get_logger(),"线速度: %.2f,角速度: %.2f",twist.linear.x,twist.angular.z);
        }
        // 3-4. 关闭文件
        reader->close();
    }
private:
    std::unique_ptr<Reader> reader;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Bag_reader>());
    rclcpp::shutdown();
    return 0;
}