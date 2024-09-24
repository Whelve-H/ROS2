cmake_minimum_required(VERSION 3.8)
project(exercise)
# /home/ros/ROS2/communication_mechanism/install/setup.bash
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
set(base_interface_demo_DIR "/home/ros/ROS2/communication_mechanism/install/base_interface_demo/share/base_interface_demo/cmake")
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(turtlesim REQUIRED)
find_package(base_interface_demo REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp_action REQUIRED)

add_executable(exe_pub_sub src/exe_pub_sub.cpp)
add_executable(exe_action src/exe_action.cpp)

target_include_directories(exe_pub_sub PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)
target_include_directories(exe_action PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include>)

target_compile_features(exe_pub_sub PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17
target_compile_features(exe_action PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(
  exe_pub_sub
  "rclcpp"
  "turtlesim"
  "base_interface_demo"
  "geometry_msgs"
  "rclcpp_action"
)
ament_target_dependencies(
  exe_action
  "rclcpp"
  "turtlesim"
  "base_interface_demo"
  "geometry_msgs"
  "rclcpp_action"
)
install(TARGETS exe_pub_sub
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS exe_action
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)
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
