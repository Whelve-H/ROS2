/*
    需求: 录制控制乌龟的速度指令
    流程:
        1. 包含头文件
        2. 初始化ROS2的客户端
        3. 自定义节点
            3-1. 创建录制对象
            3-2. 设置磁盘文件
            3-3. 写入数据(创建一个速度的订阅方,回调函数执行写入操作)
        4. 调用spain函数,并且传入节点对象指针
        5.资源的释放

*/

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rosbag2_cpp/writer.hpp"
using rosbag2_cpp::Writer;
using geometry_msgs::msg::Twist;
class Bag_writer : public rclcpp::Node
{
public:
    Bag_writer() : Node("Bag_writer_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Bag_writer 节点创建成功!");
        // 3-1. 创建录制对象
        writer = std::make_unique<Writer>();
        // 3-2. 设置磁盘文件
        writer->open("my_bag");
        // 3-3. 写入数据(创建一个速度的订阅方,回调函数执行写入操作)
        sub = this->create_subscription<Twist>("/turtle1/cmd_vel",10,std::bind(&Bag_writer::writer_callback,this,std::placeholders::_1));
    }
private:
void writer_callback(std::shared_ptr<rclcpp::SerializedMessage> msg)
{
    /*
    void write(
    std::shared_ptr<rclcpp::SerializedMessage> message, 
    const std::string &topic_name, 
    const std::string &type_name, 
    const rclcpp::Time &time)
    */
    RCLCPP_INFO(this->get_logger(),"数据写入......");
    this->writer->write(msg,"/turtle1/cmd_vel","geometry_msgs/msg/Twist",this->now());
}
// unique_ptr
    std::unique_ptr<Writer>  writer;
    rclcpp::Subscription<Twist>::SharedPtr sub;
    // rclcpp::Publisher<>
    rclcpp::Time time;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Bag_writer>());
    rclcpp::shutdown();
    return 0;
}