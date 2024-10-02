#include "rclcpp/rclcpp.hpp"

class Exer_tf_listener : public rclcpp::Node {
public:
    Exer_tf_listener() : Node("Exer_tf_listener_cpp") {
        RCLCPP_INFO(this->get_logger(), "Exer_tf_listener 节点创建成功!");
    }
};

int main(int argc, char **argv) {
    // 初始化节点
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Exer_tf_listener>());
    rclcpp::shutdown();
    return 0;
}