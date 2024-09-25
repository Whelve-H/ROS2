from launch import LaunchDescription
from launch_ros.actions import Node

# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
# from launch.actions import DeclareLaunchArgument
# from launch.substitutions import LaunchConfiguration
# 文件包含相关-------------------
# from launch.actions import IncludeLaunchDescription
# from launch.launch_description_sources import PythonLaunchDescriptionSource
# 分组相关----------------------
# from launch_ros.actions import PushRosNamespace
# from launch.actions import GroupAction
# 事件相关----------------------
"""
OnProcessStart: 当指定的进程启动时触发的事件，可以用来执行特定操作，比如记录日志或启动其他进程。

OnProcessExit: 当指定的进程退出时触发的事件，适合用于清理资源或记录退出状态。

ExecuteProcess: 用于在 Launch 文件中执行外部命令或脚本，通常用于启动非 ROS 进程。

RegisterEventHandler: 用于注册事件处理程序，以便对特定事件（如进程启动或退出）作出反应。

LogInfo:                用于记录信息到日志中，可以在启动过程中输出状态信息。
"""
from launch.event_handlers import OnProcessStart, OnProcessExit
from launch.actions import ExecuteProcess, RegisterEventHandler, LogInfo

# 获取功能包下share目录路径-------
# from ament_index_python.packages import get_package_share_directory

'''
需求: 为turtlesim_node绑定事件,节点启动,执行新生成的乌龟小程序,节点关闭的时候,执行日志输出
'''
def generate_launch_description():
    turtle = Node(
        package="turtlesim",
        executable="turtlesim_node"
    )
# ros2 service call /spawn turtlesim/srv/Spawn "{"x": 8,"y": 5,"theta": 180,"name": haha}"
    spawn =ExecuteProcess(
        cmd=["ros2 service call /spawn turtlesim/srv/Spawn \"{\"x\": 8,\"y\": 5,\"theta\": 180,\"name\": haha}\""],
        output = "both",
        shell = True
    )
    # 注册事件1
    event_start = RegisterEventHandler(
        event_handler=OnProcessStart(
            target_action=turtle,
            on_start= spawn,
            # LogInfo= 
            ) 
    )
    # 注册事件2
    event_exit = RegisterEventHandler(
        event_handler=OnProcessExit(
            target_action=turtle,
            on_exit=[LogInfo(msg="turtlesim_node退出!")]
        )
    ) 
    return LaunchDescription([turtle,event_start,event_exit])
