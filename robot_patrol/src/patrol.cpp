#include "geometry_msgs/msg/detail/twist__struct.h"
#include "rclcpp/callback_group.hpp"
#include "rclcpp/executors.hpp"
#include "rclcpp/executors/multi_threaded_executor.hpp"
#include "rclcpp/logging.hpp"
#include "rclcpp/node.hpp"
#include "rclcpp/subscription_options.hpp"
#include "sensor_msgs/msg/detail/laser_scan__struct.hpp"
#include <functional>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg//laser_scan.hpp>



class Patrol: public rclcpp::Node{
public:
Patrol():Node("patrol_node"){
callback_group_scan = this->create_callback_group(
    rclcpp::CallbackGroupType::MutuallyExclusive);
rclcpp::SubscriptionOptions option1;
option1.callback_group = callback_group_scan;  
cmd_pub = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);  
laser_sub = this->create_subscription<sensor_msgs::msg::LaserScan>("scan",10, std::bind(&Patrol::laser_callback, this, std::placeholders::_1), option1); 

}
private:
void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
  auto twist = geometry_msgs::msg::Twist();  
//   float front_read = msg->r
  RCLCPP_INFO(this->get_logger(), "clear distance infront of the robot: %f", msg->ranges[360]);
  if(msg->ranges[360] < 0.35){
//   find safe direction
 twist.linear.x = 0;
 cmd_pub->publish(twist);
  }
  else {
//   move forward
  twist.linear.x = 0.1;
  cmd_pub->publish(twist);
  }
}
rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
rclcpp::CallbackGroup::SharedPtr callback_group_scan;
rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub;

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