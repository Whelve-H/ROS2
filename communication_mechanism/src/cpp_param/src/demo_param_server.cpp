//  _    _  __     __  _
// | |  | | \ \   / / | |
// | |__| |  \ \_/ /  | |
// |  __  |   \   /   | |
// | |  | |    | |    | |____
// |_|  |_|    |_|    |______|
#include "rclcpp/rclcpp.hpp"
#include "iostream"
#define RED "\033[31m"
class Param_Server : public rclcpp::Node
{
public:
    // 如果允许参数删除,需要通过 NodeOptions 声明,删除 set_parameter 的参数
    Param_Server() : Node("Param_Server_node_cpp", rclcpp::NodeOptions().allow_undeclared_parameters(true))
    {

        RCLCPP_INFO(this->get_logger(), "Param_Server 节点创建成功!");
        RCLCPP_INFO(this->get_logger(), RED R"(
  ____          _      __  __           _            
 / ___|___   __| | ___|  \/  | __ _ ___| |_ ___ _ __ 
| |   / _ \ / _` |/ _ \ |\/| |/ _` / __| __/ _ \ '__|
| |__| (_) | (_| |  __/ |  | | (_| \__ \ ||  __/ |   
 \____\___/ \__,_|\___|_|  |_|\__,_|___/\__\___|_|
    )");
    }
    // 增
    void declare_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------增----------------------");
        this->declare_parameter("car_name", "Audi");
        this->declare_parameter("car_wheels", 4);
        this->declare_parameter("car_width", 2.1);
        // this->declare_parameters("car",
        //                           {{"car_name", "car_wheels"},
        //                            {"car_name", "Audi"},
        //                            {"car_wheels", 4}});
        // auto car = this->declare_parameters("car", {{"car_wheels", "car_width"}, {"car_wheels", 4}, {"car_width", 1.8}}, true);
        // rclcpp::NodeOptions().allow_undeclared_parameters(true) 这使用的时候才可以使用set_parameter
        this->set_parameter(rclcpp::Parameter("car_producer", "China"));
    }
    // 删
    void delete_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------删----------------------");
        // this->undeclare_parameter("car_name");   不能删除声明的参数
        if (!this->has_parameter("car_name"))
        {
            RCLCPP_INFO(this->get_logger(), "car_name 删除成功!");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "car_name 删除失败!");
        }
        this->undeclare_parameter("car_producer");
        if (!this->has_parameter("car_producer"))
        {
            RCLCPP_INFO(this->get_logger(), "car_producer 删除成功!");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "car_producer 删除失败!");
        }
        
    }
    // 改
    void update_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------改----------------------");
        this->set_parameter(rclcpp::Parameter("car_name", "Bmw"));
        RCLCPP_INFO(this->get_logger(), "car_name 修改成功!");
        rclcpp::Parameter car_n = this->get_parameter("car_name");
        RCLCPP_INFO(this->get_logger(), "key : %s ,value : %s", car_n.get_name().c_str(), car_n.as_string().c_str());
    }
    // 查
    void get_param()
    {
        RCLCPP_INFO(this->get_logger(), RED "-------------------查----------------------");
        rclcpp::Parameter car_n = this->get_parameter("car_name");
        RCLCPP_INFO(this->get_logger(), "key : %s ,value : %s", car_n.get_name().c_str(), car_n.as_string().c_str());
        auto params = this->get_parameters({"car_name", "car_wheels", "car_width"});
        // auto& 的含义
        // auto：   自动类型推导关键字。编译器会根据初始化表达式的类型自动推导变量的类型。
        // &：      引用符号，表示变量是一个引用，而不是一个值的副本。
        // 当你写 auto& 时，编译器会自动推导变量的类型，并将其声明为一个引用。这样做的目的是避免对对象的拷贝操作，直接操作原始对象，从而提高性能，特别是当处理大型对象时。
        for (auto &param : params)
        {
            RCLCPP_INFO(this->get_logger(), "key:%s ,value:%s", param.get_name().c_str(), param.value_to_string().c_str());
        }
        RCLCPP_INFO(this->get_logger(), "是否包含car_name?  %d", this->has_parameter("car_name"));
        RCLCPP_INFO(this->get_logger(), "是否包含car_price?  %d", this->has_parameter("car_price"));
    }
};

int main(int argc, char **argv)
{
    // 初始化节点
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Param_Server>();
    node->declare_param();
    node->get_param();
    node->update_param();
    node->delete_param();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}