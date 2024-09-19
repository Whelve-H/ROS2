// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from base_interface_demo:srv/Sum.idl
// generated code does not contain a copyright notice

#ifndef BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__STRUCT_H_
#define BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in srv/Sum in the package base_interface_demo.
typedef struct base_interface_demo__srv__Sum_Request
{
  int32_t num1;
  int32_t num2;
} base_interface_demo__srv__Sum_Request;

// Struct for a sequence of base_interface_demo__srv__Sum_Request.
typedef struct base_interface_demo__srv__Sum_Request__Sequence
{
  base_interface_demo__srv__Sum_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} base_interface_demo__srv__Sum_Request__Sequence;


// Constants defined in the message

/// Struct defined in srv/Sum in the package base_interface_demo.
typedef struct base_interface_demo__srv__Sum_Response
{
  int32_t sum;
} base_interface_demo__srv__Sum_Response;

// Struct for a sequence of base_interface_demo__srv__Sum_Response.
typedef struct base_interface_demo__srv__Sum_Response__Sequence
{
  base_interface_demo__srv__Sum_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} base_interface_demo__srv__Sum_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // BASE_INTERFACE_DEMO__SRV__DETAIL__SUM__STRUCT_H_
