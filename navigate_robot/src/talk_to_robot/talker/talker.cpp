#include "talker.h"
#include <iostream>
#include <memory>

Talker::Talker() : rclcpp::Node("talker") {
   this->publisher_ = this->create_publisher<geometry_msgs::msg::Pose>("poses", 10);

   this->ask_for_points();
} 

void Talker::ask_for_points(){
   while (true) {
      float x;
      float y;

      std::cout << "Insira coordenada x: " << std::endl;
      std::cin >> x;
      RCLCPP_INFO(this->get_logger(), "Variável x pega com sucesso!");

      std::cout << "Insira coordenada y: " << std::endl;
      std::cin >> y;
      RCLCPP_INFO(this->get_logger(), "Variável y pega com sucesso!");

      if (!x || !y) {
         continue;
      }

      auto pose = std::make_unique<geometry_msgs::msg::Pose>();
      pose->position.x = x; //3.7
      pose->position.y = y; //1.65
      pose->position.z = 0.0f;

      this->publisher_->publish(std::move(pose));
   }
};