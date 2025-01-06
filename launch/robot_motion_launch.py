import launch
from launch import LaunchDescription
from launch.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='assignment2_rt_ros2',
            executable='robot_motion',
            name='robot_motion_node',
            output='screen',
            parameters=[{'some_parameter': 'value'}]  # Optional parameters
        ),
    ])
