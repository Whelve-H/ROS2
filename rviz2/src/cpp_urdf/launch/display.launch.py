from launch import LaunchDescription
from launch_ros.actions import Node
# 封装终端指令相关类--------------
# from launch.actions import ExecuteProcess
# from launch.substitutions import FindExecutable
# 参数声明与获取-----------------
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
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
'''
需求: 加载urdf文件并在rviz2中显示机器人模型
核心:
    1.启动robot_state_publisher节点,该点主要以参数的方式加载urdf文件内容;
    2.启动rviz2节点
优化:
    1.添加 joint_state_publisher节点(当前机器人有非固定节点时候必须包含该节点)
    2.设置 rviz2 的默认配置文件
    3.动态传入urdf文件,把urdf 文件封装为参数      
'''
from launch_ros.descriptions import ParameterValue
from launch.substitutions import Command
def generate_launch_description():
    # 1.启动robot_state_publisher节点,该点主要以参数的方式加载urdf文件内容;
    # 获取 URDF 文件的路径
    # `ros2 pkg prefix --share cpp_urdf`/urdf/urdf/demo_1.urdf  =  urdf_file
    urdf_file = get_package_share_directory("cpp_urdf") + "/urdf/urdf/demo_1.urdf"
    model  = DeclareLaunchArgument(name="model",default_value=urdf_file)
    p_value =  ParameterValue(Command(["xacro ",LaunchConfiguration("model")]))
    robot_state_pub = Node(
        package="robot_state_publisher",
        executable= "robot_state_publisher",
        parameters =[{"robot_description":p_value}] 
    )
    # 优化1
    joint_state_pub = Node(
        package="joint_state_publisher",
        executable= "joint_state_publisher",
    )   
    # 2.启动rviz2节点
    rviz2_file = get_package_share_directory("cpp_urdf") + "/rviz/urdf.rviz"
    rviz2 = Node(
        package= "rviz2",
        executable= "rviz2",
        arguments=["-d",rviz2_file]
    )
    return LaunchDescription([model,robot_state_pub,joint_state_pub,rviz2])