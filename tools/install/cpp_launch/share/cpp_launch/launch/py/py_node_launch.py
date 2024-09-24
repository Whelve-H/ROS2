from launch import LaunchDescription
from launch_ros.actions import Node
import os
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
# from launch.event_handlers import OnProcessStart, OnProcessExit
# from launch.actions import ExecuteProcess, RegisterEventHandler,LogInfo
# 获取功能包下share目录路径-------
from ament_index_python.packages import get_package_share_directory

# def __init__(
#     self, *,
#     executable: SomeSubstitutionsType,                                //可执行程序
#     package: Optional[SomeSubstitutionsType] = None,                  //功能包名字
#     name: Optional[SomeSubstitutionsType] = None,                     //节点名称
#     namespace: Optional[SomeSubstitutionsType] = None,                //设置命名空间
#     exec_name: Optional[SomeSubstitutionsType] = None,                //设置程序标签
#     parameters: Optional[SomeParameters] = None,                      //传参      xx  yy  zz  --ros-args  
#     remappings: Optional[SomeRemapRules] = None,                      //实现话题重映射
#     ros_arguments: Optional[Iterable[SomeSubstitutionsType]] = None,  //传参      --ros-args xx yy zz   
#     arguments: Optional[Iterable[SomeSubstitutionsType]] = None,      // 
#     **kwargs
# ) -> None:
def generate_launch_description():
    print(os.path)
    t1 = Node(
        package= "turtlesim",
        executable="turtlesim_node",
        exec_name="my_label",
    )
    t2 = Node(
        package= "turtlesim",
        executable="turtlesim_node",
        name= "haha",
        namespace="t2",
        # 方式一
        # parameters=[{"background_r":255,"background_g":0,"background_b":0}]
        # ros_arguments=["--remap","__ns:=/turtle_2"]
        # 方式二： 读取yaml文件  相对路径
        parameters=[os.path.join(get_package_share_directory("cpp_launch"),"config","demo_1.launch.yaml")]
    )  
    # t2 = Node(package= "turtlesim",executable="turtlesim_node",name="t2")
    return LaunchDescription([t1,t2])
