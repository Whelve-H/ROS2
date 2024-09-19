#include "base_interface_demo/srv/sum.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
using base_interface_demo::srv::Sum;
using std::placeholders::_1;
using std::placeholders::_2;

class Server : public rclcpp::Node
{
public:
  Server() : Node("Server_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Server_node_cpp 节点创建");
    server = this->create_service<Sum>("sum", std::bind(&Server::server_callback, this, _1, _2));
    RCLCPP_INFO(this->get_logger(), "Sum 服务端启动完毕,等待请求提交......");
    // rclcpp::WallTimer<CallbackT, nullptr>::SharedPtr create_wall_timer<DurationRepT, DurationT, CallbackT>(std::chrono::duration<DurationRepT, DurationT> period, CallbackT callback, rclcpp::CallbackGroup::SharedPtr group = nullptr)
  }

private:
  rclcpp::Service<Sum>::SharedPtr server;
  void server_callback(const Sum::Request::SharedPtr req, const Sum::Response::SharedPtr res)
  {
    res->sum = req->num1 + req->num2;
    RCLCPP_INFO(this->get_logger(), "请求数据(%d,%d),相应结果为%d;", req->num1, req->num2, res->sum);
  }

  size_t count = 0;
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Server>());
  // 资源的释放
  rclcpp::shutdown();
  return 0;
}
