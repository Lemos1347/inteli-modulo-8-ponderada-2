#ifndef VALLETCLASS_H
#define VALLETCLASS_H

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_msgs/msg/string.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "slam_toolbox/srv/deserialize_pose_graph.hpp"
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include <vector>

class Vallet : public rclcpp::Node {
   public:
      Vallet();
   private:
      bool ocuppied_;
      rclcpp_action::Client<nav2_msgs::action::NavigateToPose>::SharedPtr action_client_nav2_;
      rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_;
      rclcpp::TimerBase::SharedPtr timer_;
      std::vector<std::shared_ptr<geometry_msgs::msg::PoseStamped>> poses_to_go_;
      rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr initial_pose_publisher_;

      rclcpp::Client<slam_toolbox::srv::DeserializePoseGraph>::SharedPtr load_map_client_; 

      void publishInitialPose();

      void publishMap();

      void subscription_callback(const geometry_msgs::msg::Pose::SharedPtr msg);
      geometry_msgs::msg::PoseStamped create_pose_stamped(float pos_x, float pos_y, float rot_z);

      void timer_callback();
      
      void send_goal(const geometry_msgs::msg::PoseStamped &goal_pose);
      void result_send_goal_callback(const rclcpp_action::ClientGoalHandle<nav2_msgs::action::NavigateToPose>::WrappedResult & result);
};


#endif // VALLETCLASS_H


// Esquema de metodos
// - Escuta um tópico e armazena em uma fila as posições
// - Timer para ficar olhando a fila, se tiver alguma posição mandar para o nav2
// - Mandar para o nav2 a posição