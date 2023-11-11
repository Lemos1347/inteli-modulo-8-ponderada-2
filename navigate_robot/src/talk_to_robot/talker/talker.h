#ifndef TALKERCLASS_H
#define TALKERCLASS_H

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"

class Talker : public rclcpp::Node {
   public:
      Talker();
      void ask_for_points();
   
   private:
      rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr publisher_;
};


#endif // TALKERCLASS_H