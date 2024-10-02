# from launch import LaunchDescription
# from launch_ros.actions import Node
# # 封装终端指令相关类--------------
# # from launch.actions import ExecuteProcess
# # from launch.substitutions import FindExecutable
# # 参数声明与获取-----------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# # 文件包含相关-------------------
# # from launch.actions import IncludeLaunchDescription
# # from launch.launch_description_sources import PythonLaunchDescriptionSource
# # 分组相关----------------------
# # from launch_ros.actions import PushRosNamespace
# # from launch.actions import GroupAction
# # 事件相关----------------------
# # from launch.event_handlers import OnProcessStart, OnProcessExit
# # from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# # 获取功能包下share目录路径-------
# # from ament_index_python.packages import get_package_share_directory

# def generate_launch_description():
#     # 生成一直乌龟
#     t1_name = DeclareLaunchArgument(name="turtle_1_name",default_value="turtle1")
#     t2_name = DeclareLaunchArgument(name="turtle_2_name",default_value="turtle2")
#     t1 = Node(
#         package= "turtlesim",
#         executable= "turtlesim_node",
#         remappings=[
#                 ('/turtle1/cmd_vel', '//cmd_vel')
#             ]
#     )
#     # t2 = Node(
#     #     package= "turtlesim",
#     #     executable= "turtlesim_node",
#     #     remappings=[
#     #             ('/turtle1/cmd_vel', f'/{LaunchConfiguration("turtle_2_name")}/cmd_vel')
#     #         ]
#     # )
#     return LaunchDescription([t1_name,t2_name,t1])