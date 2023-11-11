#include "talker/talker.h"

int main(int argc, char **argv)
{
   rclcpp::init(argc, argv);
   auto talker = std::make_shared<Talker>();
   rclcpp::spin(talker);
   rclcpp::shutdown();

   return 0;
}