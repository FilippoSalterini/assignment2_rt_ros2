/*Implement in ROS2, and with the new robot and simulation environment, the same behaviour implemented with the turtlesim.
The robot is already spawned in (2,2), so there is no need to use services here! You just need to develop a ROS2 package with
one node capable of moving the robot around in the simulation environment*/

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>

#include "std_msgs/msg/Float32MultiArray.h" //examRT (check later)
#include <std_srvs/srv/SetBool.hpp> //for the service part

#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

//examRT
//here some gbal variables to store values
double prev_x = 0.0;
double prev_y = 0.0;

      //ExamRT
      //function for converting feet in mt
double meterstoFeet(double meters) {
      return meters * 3.28084;
      }
    

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

      //ExamRT
      double dx = metersToFeet(x - prev_x);
      double dy = metersToFeet(y - prev_y);
      auto position_msg = std_msgs::msg::Float32MultiArray(); 
      position_msg.data.push_back(dx); 
      position_msg.data.push_back(dy); 

     position_pub->publish(position_msg);

    // Print the position and velocity
    cout << "[Robot Position]\n X: " << x << ", Y: " << y << ", Z: " << z << endl;
    cout << "[Robot Velocity]\n Linear: " << linear_velocity << ", Angular: " << angular_velocity<< endl;
    cout << "[Change of the position (feet)]\n Delta X: " << dx << ", Delta Y: " << dy << endl;

}

//examRT
// (check better later)
bool robot_control_callback(const std_srvs::srv::SetBool::Request::SharedPtr req, std_srvs::srv::SetBool::Response::SharedPtr res) {
  auto msg = geometry_msgs::msg::Twist();

  if (req->data) {
    //here start the robot
    //check here

  } else {
    //here stop the robot
    cout << "Stopping the robot" << endl;
    msg.linear.x = 0;
    msg.angular.z = 0;
  }

  // Publish the stop/start command
  cmd_vel_pub->publish(msg);

  res->success = true;
  return true;
}


//examRT
//function for converting feet in mt
double meterstoFeet(double meters) {
  return meters * 3.28084;
}


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
     
    //here create a node
    rclcpp::Node::SharedPtr node = std::make_shared<rclcpp::Node>("robot_mover");

    //publisher for the 'cmd_vel' topic
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub =
        node->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        
        
        
    //examRT   
    //publisher for the robot position in feet
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr position_pub = 
    node->create_publisher<std_msgs::msg::Float32MultiArray>("robot_position_feet", 10);
    
    //service for starting and stopping it (check)
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr robot_control_service =
      node->create_service<std_srvs::srv::SetBool>(
          "robot_control", &robot_control_callback); 

    
    
    //subscriber for the 'odom' topic
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub =
        node->create_subscription<nav_msgs::msg::Odometry>(
            "/odom", 10, odom_callback);

    auto msg = geometry_msgs::msg::Twist();

    string direction;
    double speed = 0.2; //checked on internet is a default speed for robots (?)
    
    cout <<"LIST OF POSSIBLE DIRECTIONS \n"<< endl;
    cout <<" [forward]\n [backward]\n [left]\n [right]\n"<<endl; 
    
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
        msg.linear.x = 1;        // Stop moving forward/backward
        msg.angular.z = speed; 
    }
    else if (direction == "right")
    {
        msg.linear.x = 1;        // Stop moving forward/backward
        msg.angular.z = speed; 
    }
    else
    {
        cout << "Invalid direction entered, defaulting to forward." << endl;
        msg.linear.x = speed;
    }
    // Publish the movement message
    cmd_vel_pub->publish(msg);
    
    rclcpp::Rate loop_rate(10);

    // Run for 5 seconds and process callbacks then stop
    for (int i = 0; i < 50; ++i)
    {
        rclcpp::spin_some(node); //odometry callback
        loop_rate.sleep();
    }
    
    // Stop the robot
    msg.linear.x = 0;
    msg.angular.z = 0;
    cmd_vel_pub->publish(msg);

    cout << "Robot stopped, exiting from the node..." << endl;

    rclcpp::shutdown();
    return 0;
}
