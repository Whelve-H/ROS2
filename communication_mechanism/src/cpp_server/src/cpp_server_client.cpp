#include "base_interface_demo/srv/sum.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;
using base_interface_demo::srv::Sum;
class Client : public rclcpp::Node
{
public:
  Client() : Node("Client_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Client_node_cpp 节点创建");
    client = this->create_client<Sum>("sum");
  }
  // 等待服务器的响应
  bool connect_server()
  {
    //
    while (!client->wait_for_service(1s))
    {
      if (!rclcpp::ok())
      {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "强制退出!");
        return false;
      }
      RCLCPP_WARN(this->get_logger(), "连接服务器,请稍等......");
    }
    return true;
  }
  // 组织请求数据并且发送
  rclcpp::Client<Sum>::FutureAndRequestId send_request(int num1, int num2)
  {
    auto req = std::make_shared<Sum::Request>();
    req->num1 = num1;
    req->num2 = num2;
    // 发送req数据,并且返回一个状态码
    return client->async_send_request(req);
  }

private:
  rclcpp::Client<Sum>::SharedPtr client;
};
// bool is_num(const char* str)
// {

// }
int main(int argc, char *argv[])
{
  if (argc != 3 )
  {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "传入参数有误!");
    return 0;
  }
  // || typeid(atoi(argv[1])) != typeid(int) || typeid(atoi(argv[2])) != typeid(int)
  rclcpp::init(argc, argv);
  auto client = std::make_shared<Client>();
  bool flag = client->connect_server();
  if (!flag)
  {
    RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "连接失败!");
    return 0;
  }
  // atoi char* 转换为 int
  auto res = client->send_request(atoi(argv[1]), atoi(argv[2]));
  if (rclcpp::spin_until_future_complete(client, res) == rclcpp::FutureReturnCode::SUCCESS)
  {
    RCLCPP_INFO(client->get_logger(), "请求处理成功!");
    // res.get()->sum 获取res的指针
    RCLCPP_INFO(client->get_logger(), "响应结果为:%d", res.get()->sum);
  }
  else
  {
    RCLCPP_WARN(client->get_logger(), "请求异常!");
  }
  // 资源的释放
  rclcpp::shutdown();
  return 0;
}
