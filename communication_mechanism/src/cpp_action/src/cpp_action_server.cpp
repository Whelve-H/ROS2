#include "rclcpp/rclcpp.hpp"
#include "base_interface_demo/action/progress.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using namespace std::chrono_literals;
using base_interface_demo::action::Progress;
using std::placeholders::_1;
using std::placeholders::_2;
class Action_server : public rclcpp::Node

/*
分析:
  创建动作服务端对象
  (对应的函数处理)
    处理提交的目标值
    生成连续反馈
    响应最终结果
  处理取消的处理
*/
{
public:
  Action_server() : Node("Action_server_node_cpp")
  {
    RCLCPP_INFO(this->get_logger(), "Action_server_node_cpp 节点创建成功!");
    // 返回值:  rclcpp_action::Server<ActionT>::SharedPtr
    //  NodeT node,
    //  const std::string &name,
    //  rclcpp_action::Server<ActionT>::GoalCallback handle_goal,         提交处理数据
    //  rclcpp_action::Server<ActionT>::CancelCallback handle_cancel,     取消任务
    //  rclcpp_action::Server<ActionT>::AcceptedCallback handle_accepted,  最终响应
    //  const rcl_action_server_options_t &options = rcl_action_server_get_default_options(),
    //  rclcpp::CallbackGroup::SharedPtr group = nullptr

    action_server = rclcpp_action::create_server<Progress>(
        this,
        "get_sum",
        std::bind(&Action_server::Goal_Callback, this, _1, _2),
        std::bind(&Action_server::Cancel_Callback, this, _1),
        std::bind(&Action_server::Accepted_Callback, this, _1));
  }

private:
  // /// Signature of a callback that accepts or rejects goal requests.
  // using GoalCallback = std::function<GoalResponse(const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>;
  rclcpp_action::GoalResponse Goal_Callback(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const Progress::Goal> goal)
  {
    (void)uuid;
    if (goal->num <= 1)
    {
      RCLCPP_WARN(this->get_logger(), "提交的目标值必须大于1!");
      return rclcpp_action::GoalResponse::REJECT;
    }
    RCLCPP_INFO(this->get_logger(), "提交数据成功!");
    /*
    rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    rclcpp_action::GoalResponse::ACCEPT_AND_DEFER;
    rclcpp_action::GoalResponse::REJECT;
    */
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }
  // /// Signature of a callback that accepts or rejects requests to cancel a goal.
  // using CancelCallback = std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>;
  rclcpp_action::CancelResponse Cancel_Callback(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> cancel)
  {
    (void)cancel;
    RCLCPP_INFO(this->get_logger(), "接收到取消请求!");
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  
  void execute(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> accept)
  {
    // 1. 生成连续反馈返回给客户端
    int num = accept->get_goal()->num;
    int sum = 0;
    auto feedback = std::make_shared<Progress::Feedback>();
    // 设置休眠
    rclcpp::Rate rate(2);
    for (int i = 0; i < num; i++)
    {
    // 判断是否接受到了取消请求
    //  void canceled(std::shared_ptr<base_interface_demo::action::Progress_Result> result_msg)
      if (accept->is_canceling())
      {
        RCLCPP_WARN(this->get_logger(), "任务被取消!");
        auto result = std::make_shared<Progress::Result>();
        result->sum = sum;
        accept->canceled(result);
        // 如果接受到了,终止程序
        return;
      }
      sum += i;
      double progress = i / (double)num;
      // void publish_feedback(std::shared_ptr<typename ActionT::Feedback> feedback_msg)
      feedback->progress = progress;
      accept->publish_feedback(feedback);
      RCLCPP_INFO(this->get_logger(), "当前进度为: %.2lf%%", progress * 100);
      rate.sleep();
    }
    // 2. 生成连续响应结果
    // void succeed(std::shared_ptr<base_interface_demo::action::Progress_Result> result_msg)
    // if(rclcpp::ok())
    // {
    auto result = std::make_shared<Progress::Result>();
    result->sum = sum;
    accept->succeed(result);
    // RCLCPP_INFO(this->get_logger(), "最终响应结果%d", sum);
    return;
    // }
  }
  // /// Signature of a callback that is used to notify when the goal has been accepted.
  // using AcceptedCallback = std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>;
  void Accepted_Callback(std::shared_ptr<rclcpp_action::ServerGoalHandle<Progress>> accept)
  {
    // 新建子线程处理耗时的主逻辑实现
    std::thread(std::bind(&Action_server::execute, this, accept)).detach();
  }
  
  rclcpp_action::Server<Progress>::SharedPtr action_server;
};
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Action_server>());
  return 0;
}



// #include "rclcpp/rclcpp.hpp"
// #include "rclcpp_action/rclcpp_action.hpp"
// #include "base_interface_demo/action/progress.hpp"

// using namespace std::placeholders;
// using base_interface_demo::action::Progress;
// using GoalHandleProgress = rclcpp_action::ServerGoalHandle<Progress>;

// // 3.定义节点类；
// class MinimalActionServer : public rclcpp::Node
// {
// public:

//   explicit MinimalActionServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
//   : Node("minimal_action_server", options)
//   {
//     // 3-1.创建动作服务端；
//     this->action_server_ = rclcpp_action::create_server<Progress>(
//       this,
//       "get_sum",
//       std::bind(&MinimalActionServer::handle_goal, this, _1, _2),
//       std::bind(&MinimalActionServer::handle_cancel, this, _1),
//       std::bind(&MinimalActionServer::handle_accepted, this, _1));
//     RCLCPP_INFO(this->get_logger(),"动作服务端创建，等待请求...");
//   }

// private:
//   rclcpp_action::Server<Progress>::SharedPtr action_server_;

//   // 3-2.处理请求数据；
//   rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID & uuid,std::shared_ptr<const Progress::Goal> goal)
//   {
//     (void)uuid;
//     RCLCPP_INFO(this->get_logger(), "接收到动作客户端请求，请求数字为 %ld", goal->num);
//     if (goal->num < 1) {
//       return rclcpp_action::GoalResponse::REJECT;
//     }
//     return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
//   }

//   // 3-3.处理取消任务请求；
//   rclcpp_action::CancelResponse handle_cancel(
//     const std::shared_ptr<GoalHandleProgress> goal_handle)
//   {
//     (void)goal_handle;
//     RCLCPP_INFO(this->get_logger(), "接收到任务取消请求");
//     return rclcpp_action::CancelResponse::ACCEPT;
//   }

//   void execute(const std::shared_ptr<GoalHandleProgress> goal_handle)
//   {
//     RCLCPP_INFO(this->get_logger(), "开始执行任务");
//     rclcpp::Rate loop_rate(10.0);
//     const auto goal = goal_handle->get_goal();
//     auto feedback = std::make_shared<Progress::Feedback>();
//     auto result = std::make_shared<Progress::Result>();
//     int64_t sum= 0;
//     for (int i = 1; (i <= goal->num) && rclcpp::ok(); i++) {
//       sum += i;
//       // Check if there is a cancel request
//       if (goal_handle->is_canceling()) {
//         result->sum = sum;
//         goal_handle->canceled(result);
//         RCLCPP_INFO(this->get_logger(), "任务取消");
//         return;
//       }
//       feedback->progress = (double_t)i / goal->num;
//       goal_handle->publish_feedback(feedback);
//       RCLCPP_INFO(this->get_logger(), "连续反馈中，进度：%.2f", feedback->progress);

//       loop_rate.sleep();
//     }

//     if (rclcpp::ok()) {
//       result->sum = sum;
//       goal_handle->succeed(result);
//       RCLCPP_INFO(this->get_logger(), "任务完成！");
//     }
//   }

//   // 3-4.生成连续反馈。
//   void handle_accepted(const std::shared_ptr<GoalHandleProgress> goal_handle)
//   {
//     std::thread{std::bind(&MinimalActionServer::execute, this, _1), goal_handle}.detach();
//   }
// }; 

// int main(int argc, char ** argv)
// {
//   // 2.初始化 ROS2 客户端；
//   rclcpp::init(argc, argv);
//   // 4.调用spin函数，并传入节点对象指针；
//   auto action_server = std::make_shared<MinimalActionServer>();
//   rclcpp::spin(action_server);
//   // 5.释放资源。
//   rclcpp::shutdown();
//   return 0;
// }