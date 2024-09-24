#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/spawn.hpp"
#include "turtlesim/msg/pose.hpp"
#include <cmath>

using turtlesim::msg::Pose;
using turtlesim::srv::Spawn;
using namespace std::chrono;
class Exe_action : public rclcpp::Node
{
public:
    Exe_action() : Node("Exe_action_node_cpp")
    {
        RCLCPP_INFO(this->get_logger(), "Exe_action 节点创建成功!");
        // rclcpp::Client<ServiceT>::SharedPtr create_client<ServiceT>
        // (const std::string &service_name,
        // const rmw_qos_profile_t &qos_profile = rmw_qos_profile_services_default,
        // rclcpp::CallbackGroup::SharedPtr group = nullptr)
        spawn_client = this->create_client<Spawn>("/spawn");
        sub_turtle_1 = this->create_subscription<Pose>("/turtle1/pose", 10, std::bind(&Exe_action::sub_turtle_1_callback, this, std::placeholders::_1));
        sub_turtle_2 = this->create_subscription<Pose>("/turtle2/pose", 10, std::bind(&Exe_action::sub_turtle_2_callback, this, std::placeholders::_1));
    }
    void sub_turtle_1_callback(const Pose &pose)
    {
        pose_turtle_1 = std::make_shared<Pose>(pose);
    }
    void sub_turtle_2_callback(const Pose &pose)
    {
        pose_turtle_2 = std::make_shared<Pose>(pose);
    }
    bool connect_server()
    {
        //
        while (!spawn_client->wait_for_service(1s))
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
    rclcpp::Client<Spawn>::FutureAndRequestId send_request(float x, float y, float theta, char *name)
    {
        auto spawn = std::make_shared<Spawn::Request>();
        spawn->x = x;
        spawn->y = y;
        spawn->theta = theta;
        spawn->name = std::string(name);
        // rclcpp::Client<turtlesim::srv::Spawn>::SharedFutureWithRequestAndRequestId async_send_request<CallbackT, <unnamed>>(std::shared_ptr<turtlesim::srv::Spawn_Request> request, CallbackT &&cb)
        return this->spawn_client->async_send_request(spawn);
    }

    // private:
    rclcpp::Client<Spawn>::SharedPtr spawn_client;
    rclcpp::Subscription<Pose>::SharedPtr sub_turtle_1;
    rclcpp::Subscription<Pose>::SharedPtr sub_turtle_2;
    Pose::SharedPtr pose_turtle_1;
    Pose::SharedPtr pose_turtle_2;
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    auto exe_action = std::make_shared<Exe_action>();
    bool flag = exe_action->connect_server();
    if (!flag)
    {
        rclcpp::shutdown();
        return 0;
    }
    auto res = exe_action->send_request(atof(argv[1]), atof(argv[2]), atof(argv[3]), (argv[4]));
    if (rclcpp::spin_until_future_complete(exe_action, res) == rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_INFO(exe_action->get_logger(), "请求处理成功!");
        // res.get()->sum 获取res的指针
        RCLCPP_INFO(exe_action->get_logger(), "创建新的小乌龟:%s", res.get()->name.c_str());
        if (exe_action->pose_turtle_1 != nullptr && exe_action->pose_turtle_2 != nullptr)
        {
            RCLCPP_INFO(exe_action->get_logger(), "Turtle_1 乌龟的坐标(%f,%f)", exe_action->pose_turtle_1->x, exe_action->pose_turtle_1->y);
            RCLCPP_INFO(exe_action->get_logger(), "Turtle_2 乌龟的坐标(%f,%f)", exe_action->pose_turtle_2->x, exe_action->pose_turtle_2->y);
            double dir = std::sqrt((std::pow(exe_action->pose_turtle_1->x - exe_action->pose_turtle_2->x, 2)) + (pow(exe_action->pose_turtle_1->y - exe_action->pose_turtle_2->y, 2)));
            RCLCPP_INFO(exe_action->get_logger(), "两只乌龟相距:%lf", dir);
        }
        else
        {
            RCLCPP_WARN(exe_action->get_logger(), "pose_turtle_1/pose_turtle_2 初始化失败!!!");
        }
    }
    else
    {
        RCLCPP_WARN(exe_action->get_logger(), "请求异常!");
    }
    rclcpp::shutdown();
    return 0;
}