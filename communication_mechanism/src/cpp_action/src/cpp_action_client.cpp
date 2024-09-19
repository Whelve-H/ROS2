#include "rclcpp/rclcpp.hpp"
#include "base_interface_demo/action/progress.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include <csignal> // 用于捕获信号
using namespace std::chrono_literals;
using base_interface_demo::action::Progress;
using std::placeholders::_1;
using std::placeholders::_2;

class Action_client : public rclcpp::Node
{
public:
  Action_client() : Node("Action_client_node_cpp")
  {
    /*
      rclcpp_action::Client<ActionT>::SharedPtr create_client<ActionT, NodeT>(
      NodeT node,
      const std::string &name,
      rclcpp::CallbackGroup::SharedPtr group = nullptr,
      const rcl_action_client_options_t &options = rcl_action_client_get_default_options())
    */
    RCLCPP_INFO(this->get_logger(), "Action_client_node_cpp 节点创建成功!");
    action_client = rclcpp_action::create_client<Progress>(this, "get_sum");
  }

  void send_goal(int num)
  {
    while (!this->action_client->wait_for_action_server(1s))
    {
      RCLCPP_WARN(this->get_logger(), "服务器连接失败");
      return;
    }
    // 异步发送
    // std::shared_future<rclcpp_action::ClientGoalHandle<base_interface_demo::action::Progress>::SharedPtr> async_send_goal
    // (const base_interface_demo::action::Progress::Goal &goal,
    // const rclcpp_action::Client<base_interface_demo::action::Progress>::SendGoalOptions &options)
    auto goal = Progress::Goal();
    goal.num = num;
    auto options = rclcpp_action::Client<Progress>::SendGoalOptions();
    options.goal_response_callback = std::bind(&Action_client::goal_response_callback, this, _1);
    options.result_callback = std::bind(&Action_client::result_callback, this, _1);
    options.feedback_callback = std::bind(&Action_client::feedback_callback, this, _1, _2);
    // future_goal_handle = this->action_client->async_send_goal(,)
    future_goal_handle = this->action_client->async_send_goal(goal, options);
    // rclcpp::spin_until_future_complete 是 ROS 2 中 rclcpp 库的一部分，用于等待异步任务（future）完成的函数。它的主要作用是阻塞当前线程，直到给定的 future 对象完成或超时。这通常用于等待某些异步操作的结果，例如发送目标动作或服务调用的响应
    // rclcpp::spin_until_future_complete(node, goal_handle_future) != rclcpp::FutureReturnCode::SUCCESS
  }

  // void cancel_goal()
  // {
  //   auto goal_handle = future_goal_handle.get();
  //   if (!goal_handle)
  //   {
  //     RCLCPP_ERROR(this->get_logger(), "目标未被服务器接收，无法取消");
  //     return;
  //   }
  //   else
  //   {
  //     RCLCPP_INFO(this->get_logger(), "目标被服务器接收");
  //   }
  //   auto cancel_future = action_client->async_cancel_goal(goal_handle);
  //   RCLCPP_INFO(this->get_logger(), "111111111111");
  //   // auto rer = action_client->async_get_result
  //   // RCLCPP_INFO(this->get_logger(),"取消目标值为: %d",cancel_future.get()->);
  //   // if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), cancel_future) != rclcpp::FutureReturnCode::SUCCESS)
  //   // {
  //   //   RCLCPP_ERROR(this->get_logger(), "取消请求失败");
  //   // }
  //   RCLCPP_INFO(this->get_logger(), "2222222222");
  //   // if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), cancel_future) == rclcpp::FutureReturnCode::SUCCESS)
  //   // {
  //     RCLCPP_ERROR(this->get_logger(), "取消请求成功");
  //     // 检查取消的响应结果
  //     // auto cancel_result = cancel_future.get();
  //     // if (cancel_result->goals_canceling.size()>0)
  //     // {
  //     //   RCLCPP_INFO(this->get_logger(), "目标取消成功");
  //     // }
  //     // RCLCPP_INFO(this->get_logger(), "2222222222");
  //     // if (cancel_result->goals_canceling.size()<=0)
  //     // {
  //     //   RCLCPP_WARN(this->get_logger(), "取消请求未能成功，目标可能已完成或无法取消");
  //     // }
  //   // }
  // }


  void get_goal_result()
  {
    if (!future_goal_handle.valid())
    {
      RCLCPP_WARN(this->get_logger(), "目标句柄无效，无法获取结果");
      return;
    }

    // 获取目标句柄
    auto goal_handle = future_goal_handle.get();
    if (!goal_handle)
    {
      RCLCPP_ERROR(this->get_logger(), "目标未被服务器接收，无法获取结果");
      return;
    }

    // 异步获取目标结果
    auto result_future = action_client->async_get_result(goal_handle);

    // 等待结果完成
    if (rclcpp::spin_until_future_complete(this->shared_from_this(), result_future) != rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_ERROR(this->get_logger(), "获取结果请求失败");
      return;
    }

    // 获取最终结果
    auto result = result_future.get();
    // 处理不同的结果状态
    switch (result.code)
    {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(), "目标成功执行，结果是: %ld", result.result->sum);
      break;
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_WARN(this->get_logger(), "目标被中断");
      break;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(), "目标已取消");
      break;
    default:
      RCLCPP_ERROR(this->get_logger(), "目标执行失败，未知状态");
      break;
    }
  }

private:
  // void operator()(std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interface_demo::action::Progress>> __args) const
  // std::function<void (typename GoalHandle::SharedPtr)>;
  // 处理服务端对goal的响应
  void goal_response_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal)
  {
    if (!goal)
    {
      RCLCPP_ERROR(this->get_logger(), "目标被服务器拒绝!");
    }
    else
    {
      RCLCPP_INFO(this->get_logger(), "服务器成功响应!");
    }
  }
  // void operator()(std::shared_ptr<rclcpp_action::ClientGoalHandle<base_interface_demo::action::Progress>> __args, std::shared_ptr<const base_interface_demo::action::Progress_Feedback> __args) const
  //   using FeedbackCallback =
  // std::function<void (
  //     typename ClientGoalHandle<ActionT>::SharedPtr,
  //     const std::shared_ptr<const Feedback>)>;
  void feedback_callback(rclcpp_action::ClientGoalHandle<Progress>::SharedPtr goal, const std::shared_ptr<const Progress::Feedback> feedback)
  {
    (void)goal;
    auto progress = feedback->progress;
    RCLCPP_INFO(this->get_logger(), "当前进度: %d%%", (int)(progress * 100));
  }
  // void operator()(const rclcpp_action::ClientGoalHandle<base_interface_demo::action::Progress>::WrappedResult &__args) const
  // using ResultCallback = std::function<void (const WrappedResult & result)>;
  void result_callback(const rclcpp_action::ClientGoalHandle<Progress>::WrappedResult &result)
  {
    // enum class ResultCode : int8_t
    // {
    //   UNKNOWN = action_msgs::msg::GoalStatus::STATUS_UNKNOWN,
    //   SUCCEEDED = action_msgs::msg::GoalStatus::STATUS_SUCCEEDED,
    //   CANCELED = action_msgs::msg::GoalStatus::STATUS_CANCELED,
    //   ABORTED = action_msgs::msg::GoalStatus::SaTATUS_ABORTED
    // };
    auto code = result.code;
    switch (code)
    {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(), "最终响应结果: %ld", result.result->sum);
      return;
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_INFO(this->get_logger(), "被中断");
      return;
    case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_INFO(this->get_logger(), "被取消,当前结果为: %ld", result.result->sum);
      return;
    case rclcpp_action::ResultCode::UNKNOWN:
      RCLCPP_INFO(this->get_logger(), "未知异常!");
      return;
    }
  }

private:
  rclcpp_action::Client<Progress>::SharedPtr action_client;
  std::shared_future<rclcpp_action::ClientGoalHandle<Progress>::SharedPtr> future_goal_handle;
};

std::shared_ptr<Action_client> action_client = nullptr;
// void signal_handler(int signal)
// {
//   if (signal == SIGINT)
//   {
//     RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "捕获到退出信号，取消目标...");
//     if (action_client)
//     {
//       action_client->cancel_goal(); // 调用取消目标的函数
//     }
//     rclcpp::shutdown(); // 关闭ROS2
//   }
// }
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "请传入一个整数!");
    return 0;
  }
  rclcpp::init(argc, argv);
  action_client = std::make_shared<Action_client>();
  // 注册信号处理函数
  // std::signal(SIGINT, signal_handler);
  action_client->send_goal(atoi(argv[1]));
  rclcpp::spin(action_client);
  rclcpp::shutdown();
  return 0;
}
