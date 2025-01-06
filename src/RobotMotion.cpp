/*Implement in ROS2, and with the new robot and simulation environment, the same behaviour implemented with the turtlesim.
The robot is already spawned in (2,2), so there is no need to use services here! You just need to develop a ROS2 package with
one node capable of moving the robot around in the simulation environment*/

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

//void function to move the robot
//declare a publisher that is going to publish a message of type <geometry_msgs::msg::Twist>
//sharedPtr used to manage the memory of the publisher object
//ensures that the publisher object is properly cleaned up once it's no longer needed
void robot_motion(rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub) {
auto msg = geometry_msgs::msg::Twist();

    string direction;
    double speed = 0.2; //checked on internet is a default speed for robots (?)
    cout <<"LIST OF POSSIBLE DIRECTIONS \n"<<;
    cout <<" [forward]\n [backward]\n [left]\n [right]\n"; 
    cout << "Enter the direction of the robot: ";
    cin >> direction;

    std::cout << "Enter the speed (default 0.2 m/s): ";
    std::cin >> speed;

    if (direction == "forward")
    {
        msg.linear.x = speed;
    }
    else if (direction == "backward")
    {
        msg.linear.x = -speed;
    }
    else if (direction == "left")
    {
        msg.angular.z = speed;
    }
    else if (direction == "right")
    {
        msg.angular.z = -speed;
    }
    else
    {
        std::cout << "Invalid direction entered, defaulting to forward." << std::endl;
        msg.linear.x = speed;
    }

    // Publish the movement message
    cmd_vel_pub->publish(msg);
}
