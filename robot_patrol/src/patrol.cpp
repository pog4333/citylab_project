#include "rclcpp/executors.hpp"
#include "rclcpp/logging.hpp"
#include "rclcpp/node.hpp"
#include "sensor_msgs/msg/detail/laser_scan__struct.hpp"
#include <functional>
#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.h>
#include <sensor_msgs/msg//laser_scan.hpp>


class Patrol: public rclcpp::Node{
public:
Patrol():Node("patrol_node"){
laser_sub = this->create_subscription<sensor_msgs::msg::LaserScan>("scan",10, std::bind(&Patrol::laser_callback, this, std::placeholders::_1)); 
}

private:
void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg){
  RCLCPP_INFO(this->get_logger(), "clear distance infront of th robot: %f", msg->ranges[180]);
}
rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
};

int main(int argc, char * argv[]){
rclcpp::init(argc, argv);
rclcpp::spin(std::make_shared<Patrol>());
rclcpp::shutdown();
return 0;

}