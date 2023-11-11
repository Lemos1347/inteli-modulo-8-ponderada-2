#include "vallet/vallet.h"

int main(int argc, char **argv)
{
   rclcpp::init(argc, argv);
   auto vallet_node = std::make_shared<Vallet>();
   rclcpp::spin(vallet_node);
   rclcpp::shutdown();
   return 0;
}