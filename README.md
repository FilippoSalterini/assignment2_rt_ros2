# Assignment2_ROS2 - Research Track 1
Development of a ROS2 package with one node capable of moving the robot around in the simulation environment.
## Overview
This node asks the user to set a direction and a speed.  These inputs, which will be sent to the robot, will allow control of the robot itself.1.  
## Nodes
### Node description
* File name: RobotMotion.cpp
* Key Features:  
Includes :  
1.  *rclcpp/rclcpp.hpp* ROS2 C++ client library. 
2.  *geometry_msgs/msg/twist.hpp* Twist message type, used to send velocity commands to the robot (linear and angular velocities).3.
3.  *nav_msgs/msg/odometry.hpp* Message type for odometry data, which includes robot's position and velocity information.
4.  *iostream* Standard C++ library for input/output.
5.  *thread* and *chrono* Used for introducing delays (for example - sleeping for 5 seconds in the robot's motion).  


Defined a :
  1.  ```void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)```function helpful to exctract the position and velocity information of the robot (from Odometry messages), and prints them to the console.
  2.  ```void robot_motion(rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub)``` that allows the user to set a direction [forward] [backward] [left] [right] and a speed, then it sets the appropriate linear.x
and angular.z values in the Twist message and publishes it to cmd_vel to move the robot. After 5 seconds it publishes zero velocities for both linear and angular components to stop the robot.  
Then inside the *main*:  
Creates a publisher to publish type Twist messages on the /cmd_vel topic. This is also the topic which commands (set by the users) to move the robot are sent.
Also creates a subscriber to /odom topic. When new odometry data is received, the *odom_callback* function is called to print the robot's position and velocity
After the robot has stopped, *rclcpp::shutdown()* is called to shut down the ROS2 system.


## How to run it
1. First launch the gazebo simulation by using *ros2 launch robot_urdf gazebo.launch.py*
2. Run the node RobotMotion.cpp by using *ros2 run assignment2_rt_ros2 robot_motion* , the user is allowed to choose a direction and a speed, once the command is sent and robot
is moving after 5 second he stop, if the user want to choose a new direction must run again the node.

# NOTES  
The interface with the user is not perfectly programmed, it is a simple program just to set a direction and a speed,  if the user set a wrong input like
a direction that is not [forward] [backward] [left] [right] the robot, by default , will go forward.  
Also is it required to choose not high values for the speed, as said the default speed fot the robot is 0.2 (m/s)  
For other explanation check the annotations in the code.
