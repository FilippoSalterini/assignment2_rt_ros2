/*Implement in ROS2, and with the new robot and simulation environment, the same behaviour implemented with the turtlesim.
The robot is already spawned in (2,2), so there is no need to use services here! You just need to develop a ROS2 package with
one node capable of moving the robot around in the simulation environment*/

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

//void function to move the robot
//declare a publisher that is going to publish a message of type <geometry_msgs::msg::Twist>
//sharedPtr used to manage the memory of the publisher object
//ensures that the publisher object is properly cleaned up once it's no longer needed

//subsriber to print robot position and velocity
void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{ 
    //get the position
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;
    double z = msg->pose.pose.position.z;

    //get robot's velocity from the message (L and A)
    double linear_velocity = msg->twist.twist.linear.x;  // Linear velocity in x direction
    double angular_velocity = msg->twist.twist.angular.z; // Angular velocity (rotation)

    // Print the position and velocity
    cout << "[Robot Position]\n X: " << x << ", Y: " << y << ", Z: " << z << endl;
    cout << "[Robot Velocity]\n Linear: " << linear_velocity << ", Angular: " << angular_velocity<< endl;
}

void robot_motion(rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub) {

    auto msg = geometry_msgs::msg::Twist();

    string direction;
    double speed = 0.2; //checked on internet is a default speed for robots (?)
    cout <<"LIST OF POSSIBLE DIRECTIONS \n"<< endl;
    cout <<" [forward]\n [backward]\n [left]\n [right]\n"; 
    cout << "Enter the direction of the robot: ";
    cin >> direction;

    cout << "Enter the speed (default 0.2 m/s): ";
    cin >> speed;

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
        cout << "Invalid direction entered, defaulting to forward." << endl;
        msg.linear.x = speed;
    }

    // Publish the movement message
    cmd_vel_pub->publish(msg);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
     
    //here create a node
    rclcpp::Node::SharedPtr node = std::make_shared<rclcpp::Node>("robot_mover");

    //publisher for the 'cmd_vel' topic
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    
    //subscriber for the 'odom' topic
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub =
        node->create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10, odom_callback);


    robot_motion(cmd_vel_pub);

    rclcpp::spin(node); //this ensures the node to continue running
    
    rclcpp::shutdown();
    return 0;
}
