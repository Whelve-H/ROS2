cmake_minimum_required(VERSION 3.8)
project(cpp_tf_broadcaster)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)

add_executable(demo src/demo.cpp)
add_executable(cpp_static_tf_broadcaster  src/cpp_static_tf_broadcaster.cpp)
add_executable(cpp_dynamic_tf_broadcaster src/cpp_dynamic_tf_broadcaster.cpp)
add_executable(cpp_tf_point src/cpp_tf_point.cpp)

# add_executable(cpp_dynamic_tf_broadcaster src/cpp_dynamic_tf_broadcaster.cpp)
# add_executable(cpp_dynamic_tf_broadcaster src/cpp_dynamic_tf_broadcaster.cpp)
# cpp_tf_point.cpp
# cpp_static_tf_broadcaster.cpp

target_include_directories(demo PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(cpp_static_tf_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(cpp_dynamic_tf_broadcaster PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_include_directories(cpp_tf_point PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)


  # target_include_directories(cpp_static_tf_broadcaster PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)
# target_include_directories(cpp_dynamic_tf_broadcaster PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)
target_compile_features(demo PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
ament_target_dependencies(
  demo
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)
ament_target_dependencies(
  cpp_static_tf_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)
ament_target_dependencies(
  cpp_dynamic_tf_broadcaster
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)
ament_target_dependencies(
  cpp_tf_point
  "rclcpp"
  "tf2"
  "tf2_ros"
  "geometry_msgs"
  "turtlesim"
)
# ament_target_dependencies(
#   cpp_static_tf_broadcaster
#   "rclcpp"
#   "tf2"
#   "tf2_ros"
#   "geometry_msgs"
#   "turtlesim"
# )
# ament_target_dependencies(
#   cpp_dynamic_tf_broadcaster
#   "rclcpp"
#   "tf2"
#   "tf2_ros"
#   "geometry_msgs"
#   "turtlesim"
# )
install(TARGETS 
  demo
  cpp_static_tf_broadcaster
  cpp_dynamic_tf_broadcaster
  cpp_tf_point
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
