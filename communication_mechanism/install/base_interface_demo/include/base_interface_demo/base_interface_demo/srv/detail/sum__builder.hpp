// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from base_interface_demo:srv/Sum.idl
// generated code does not contain a copyright notice

#ifndef BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__BUILDER_HPP_
#define BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "base_interface_demo/srv/detail/sum__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace base_interface_demo
{

namespace srv
{

namespace builder
{

class Init_Sum_Request_num2
{
public:
  explicit Init_Sum_Request_num2(::base_interface_demo::srv::Sum_Request & msg)
  : msg_(msg)
  {}
  ::base_interface_demo::srv::Sum_Request num2(::base_interface_demo::srv::Sum_Request::_num2_type arg)
  {
    msg_.num2 = std::move(arg);
    return std::move(msg_);
  }

private:
  ::base_interface_demo::srv::Sum_Request msg_;
};

class Init_Sum_Request_num1
{
public:
  Init_Sum_Request_num1()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Sum_Request_num2 num1(::base_interface_demo::srv::Sum_Request::_num1_type arg)
  {
    msg_.num1 = std::move(arg);
    return Init_Sum_Request_num2(msg_);
  }

private:
  ::base_interface_demo::srv::Sum_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::base_interface_demo::srv::Sum_Request>()
{
  return base_interface_demo::srv::builder::Init_Sum_Request_num1();
}

}  // namespace base_interface_demo


namespace base_interface_demo
{

namespace srv
{

namespace builder
{

class Init_Sum_Response_sum
{
public:
  Init_Sum_Response_sum()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::base_interface_demo::srv::Sum_Response sum(::base_interface_demo::srv::Sum_Response::_sum_type arg)
  {
    msg_.sum = std::move(arg);
    return std::move(msg_);
  }

private:
  ::base_interface_demo::srv::Sum_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::base_interface_demo::srv::Sum_Response>()
{
  return base_interface_demo::srv::builder::Init_Sum_Response_sum();
}

}  // namespace base_interface_demo

#endif  // BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__BUILDER_HPP_
