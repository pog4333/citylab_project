#include "geometry_msgs/msg/detail/twist__struct.h"
#include "rclcpp/callback_group.hpp"
#include "rclcpp/executors.hpp"
#include "rclcpp/executors/multi_threaded_executor.hpp"
#include "rclcpp/logging.hpp"
#include "rclcpp/node.hpp"
#include "rclcpp/subscription_options.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/timer.hpp"
#include "sensor_msgs/msg/detail/laser_scan__struct.hpp"
#include <functional>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <cmath>

using namespace std::chrono_literals;


class Patrol: public rclcpp::Node{
public:
Patrol():Node("patrol_node"){


cmd_pub = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);  
laser_sub = this->create_subscription<sensor_msgs::msg::LaserScan>("scan",10, std::bind(&Patrol::laser_callback, this, std::placeholders::_1)); 

}
private:


void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
  auto twist = geometry_msgs::msg::Twist();  
  largest_distance_ = 0;

  RCLCPP_INFO(this->get_logger(), "clear distance infront of the robot: %f", msg->ranges[360]);
  for(int a = 340; a <380; a++){
    if(msg->ranges[a] < 0.35 || msg->ranges[180] < 0.1 || msg->ranges[540] < 0.1){
//   find safe direction
    for(int i=180; i<541; i++){
        if (msg->ranges[i] > largest_distance_){
        largest_distance_ = msg->ranges[i];
        direction_ = (i/2 - 180)*(M_PI/180);
        RCLCPP_INFO(this->get_logger(), "direction: %f", direction_/2);

        }
    }
    twist.linear.x = 0.1;
    twist.angular.z = direction_/2;
    cmd_pub->publish(twist);
    }
    
    else {
//   move forward
    twist.linear.x = 0.1;
    twist.angular.z = 0;
    cmd_pub->publish(twist);
    }
  }
}
rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;
float largest_distance_;
double direction_;
};

int main(int argc, char * argv[]){
rclcpp::init(argc, argv);
std::shared_ptr<Patrol> patrol_ =
    std::make_shared<Patrol>();
rclcpp::executors::MultiThreadedExecutor executor;
executor.add_node(patrol_);
executor.spin();
rclcpp::shutdown();
return 0;

}