#include "rclcpp/rclcpp.hpp"

#define RED "\033[31m"
using namespace std::chrono_literals;

class Param_Client : public rclcpp::Node
{
public:
    Param_Client() : Node("Param_Client_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Param_Client 节点创建成功!");
        praram_client = std::make_shared<rclcpp::SyncParametersClient>(this, "Param_Server_node_cpp");
    }
    bool connect_server()
    {
        int i;
        for (i = 0; i < 10; i++)
        {
            if (!rclcpp::ok())
            {
                return false;
            }
            if (!praram_client->wait_for_service(1s))
            {
                RCLCPP_WARN(this->get_logger(), "等待Praram Server响应......");
            }
            else
            {
                break;
            }
        }
        if (i == 10)
        {
            RCLCPP_ERROR(this->get_logger(), "Praram Server连接失败!");
            return false;
        };
        RCLCPP_INFO(this->get_logger(), "Praram Server连接成功!");
        return true;
    }
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------查----------------------");
        rclcpp::Parameter car_name_value = praram_client->get_parameter<rclcpp::Parameter>("car_name");
        // 获取单个键值
        RCLCPP_INFO(this->get_logger(), "key:%s ,value:%s", car_name_value.get_name().c_str(), car_name_value.value_to_string().c_str());
        // 获取多个键值
        auto car_values = praram_client->get_parameters({"car_name", "car_wheels", "car_width"});
        for (const auto &car_value : car_values)
        {
            RCLCPP_INFO(this->get_logger(), "key:%s ,value:%s", car_value.get_name().c_str(), car_value.value_to_string().c_str());
        }
        // 判读是否有某一个参数
        RCLCPP_INFO(this->get_logger(), "是否包含 'car_name'  键(%s)", praram_client->has_parameter("car_name") ? "True" : "False");
        RCLCPP_INFO(this->get_logger(), "是否包含 'car_price' 键(%s)", praram_client->has_parameter("car_price") ? "True" : "False");
    }
    void upadate_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------改 ----------------------");

        praram_client->set_parameters({
            rclcpp::Parameter("car_name","Xiaomi SU7"),
            // 当创建不纯在的键值对,要保证server option 开启
            rclcpp::Parameter("car_price",19.22)
            });
    }

private:
    rclcpp::SyncParametersClient::SharedPtr praram_client;
};
int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    auto param_client = std::make_shared<Param_Client>();
    if (param_client->connect_server())
    {
        param_client->get_param();
        param_client->upadate_param();
        param_client->get_param();
    }
    // rclcpp::spin(param_client);
    rclcpp::shutdown();
    return 0;
}