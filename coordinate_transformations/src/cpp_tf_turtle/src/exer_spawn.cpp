/*
需求: 创建一个客户端,生成一个小乌龟.
*/
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"
using turtlesim::srv::Spawn;
using namespace std::chrono_literals;
class Exer_spawn : public rclcpp::Node
{
public:
    Exer_spawn() : Node("Exer_spawn_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Exer_spawn 节点创建成功!");
        // rclcpp::Client<ServiceT>::SharedPtr create_client<ServiceT>(const std::string &service_name, const rmw_qos_profile_t &qos_profile = rmw_qos_profile_services_default, rclcpp::CallbackGroup::SharedPtr group = nullptr)
        client = this->create_client<Spawn>("/spawn");
    }

    bool connect_server()
    {
        while (!client->wait_for_service())
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
    rclcpp::Client<Spawn>::FutureAndRequestId send_request(char **argv)
    {
        Spawn::Request::SharedPtr req;
        req = std::make_shared<Spawn::Request>();
        req->x = atol(argv[1]);
        req->y = atol(argv[2]);
        req->theta = atol(argv[3]);
        req->name = argv[4];
        return (this->client->async_send_request(req));
    }

private:
    rclcpp::Client<Spawn>::SharedPtr client;
};

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请输入有效参数！！！");
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "x y theta name");
    }
    rclcpp::init(argc, argv);
    auto client = std::make_shared<Exer_spawn>();
    bool flag;
    flag = client->connect_server();
    if (!flag)
    {
        RCLCPP_ERROR(client->get_logger(), "连接服务器失!!!");
        return 0;
    }
    auto res = client->send_request(argv);
    switch (rclcpp::spin_until_future_complete(client, res))
    {
    case rclcpp::FutureReturnCode::SUCCESS:
        RCLCPP_INFO(client->get_logger(), "服务响应成功!!!");
        RCLCPP_INFO(client->get_logger(), "生成一只新的小乌龟为: %s", res.get()->name.c_str());
        break;
    // INTERRUPTED: 操作被中断，比如节点关闭时。
    case rclcpp::FutureReturnCode::INTERRUPTED:
        RCLCPP_WARN(client->get_logger(), "操作被中断!!!");
        break;
    case rclcpp::FutureReturnCode::TIMEOUT:
        RCLCPP_WARN(client->get_logger(), "等待超时!!!");
        break;
    default:
        RCLCPP_WARN(client->get_logger(), "未知异常!!!");
        break;
    }
    rclcpp::shutdown();
    return 0;
}