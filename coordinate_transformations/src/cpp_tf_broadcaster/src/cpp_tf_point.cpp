#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"
using geometry_msgs::msg::PointStamped;
using namespace std::chrono_literals;
class Cpp_tf_point : public rclcpp::Node
{
public:
    Cpp_tf_point() : Node("Cpp_tf_point_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Cpp_tf_point 节点创建成功!");
        point = {0, 0, 0};
        // 创建发布方
        // const std::string &topic_name, const rclcpp::QoS &qos
        pub = this->create_publisher<PointStamped>("point", 10);
        // 创建定时器
        // std::chrono::duration<DurationRepT, DurationT> period, CallbackT callback
        timer = this->create_wall_timer(1s, std::bind(&Cpp_tf_point::timer_callback, this)); // 回调函数中组织并且发布数据
    }

private:
    void timer_callback()
    {
        // 组织消息
        PointStamped ps;
        ps.header.stamp = this->now();
        ps.header.frame_id = "laser";
        point[0] += 0.1;
        point[1] -= 0.1;
        point[2] += 0.1;
        ps.point.x = point[0];
        ps.point.y = point[1];
        ps.point.z = point[2];

        // 发布消息
        RCLCPP_INFO(this->get_logger(), "消息成功发布！！！");
        RCLCPP_INFO(this->get_logger(), "当前point的坐标为:(%lf,%lf,%lf)", point[0], point[1], point[2]);

        pub->publish(ps);
    }
    rclcpp::Publisher<PointStamped>::SharedPtr pub;
    rclcpp::TimerBase::SharedPtr timer;
    std::vector<double> point;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Cpp_tf_point>());
    rclcpp::shutdown();
    return 0;
}