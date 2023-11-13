#include "vallet.h"
#include <memory>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <filesystem>

Vallet::Vallet() : rclcpp::Node("VALLET"), ocuppied_(false)
{

   RCLCPP_INFO(this->get_logger(), "Criando o client do nav2...");
   this->action_client_nav2_ = rclcpp_action::create_client<nav2_msgs::action::NavigateToPose>(this, "navigate_to_pose");

   RCLCPP_INFO(this->get_logger(), "Criando o subscriber to tópico poses...");
   this->subscription_ = this->create_subscription<geometry_msgs::msg::Pose>("poses", 10, std::bind(&Vallet::subscription_callback, this, std::placeholders::_1));

   RCLCPP_INFO(this->get_logger(), "Criando o timer...");
   this->timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&Vallet::timer_callback, this));

   RCLCPP_INFO(this->get_logger(), "Criando o vetor de poses...");
   this->poses_to_go_ = std::vector<std::shared_ptr<geometry_msgs::msg::PoseStamped>>();

   RCLCPP_INFO(this->get_logger(), "Iniciand o initial_pose do nav2...");
   this->initial_pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>("/initialpose", 10);

   RCLCPP_INFO(this->get_logger(), "O publisher map...");
   this->load_map_client_ = this->create_client<slam_toolbox::srv::DeserializePoseGraph>("/slam_toolbox/deserialize_map");

   while(!load_map_client_->wait_for_service(std::chrono::milliseconds(1000))) {
      RCLCPP_INFO(this->get_logger(), "Esperando o servico...");
   }

   // this->publishInitialPose();
   this->publishMap();

   // For testing ...
   // auto pose_stamped = this->create_pose_stamped(3.7f, 1.65f, 0.0f);
   // this->send_goal(pose_stamped);
}

// Para movimentacao com SLAM
void Vallet::publishMap()
{
   auto map_data = std::make_shared<slam_toolbox::srv::DeserializePoseGraph_Request>();
   std::string package_path = ament_index_cpp::get_package_share_directory("navigate_robot");
   auto path_obj = std::filesystem::path(package_path).parent_path().parent_path().parent_path().parent_path();
   auto map_path = path_obj.string() + std::string("/assets/map/map");

   RCLCPP_INFO(this->get_logger(), "Asset path: %s", map_path.c_str());
   map_data->filename = map_path;
   map_data->match_type = 1;

   RCLCPP_INFO(this->get_logger(), "Publicando o mapa...");
   auto promise = this->load_map_client_->async_send_request(map_data);
   rclcpp::spin_until_future_complete(this->get_node_base_interface(), promise);
   RCLCPP_INFO(this->get_logger(), "Mapa publicado...");
};

// Para movimentacao com base no AMCL
void Vallet::publishInitialPose()
{
   geometry_msgs::msg::PoseWithCovarianceStamped initial_pose;
   initial_pose.header.frame_id = "map";
   initial_pose.header.stamp = this->get_clock()->now();

   initial_pose.pose.pose.position.x = 0.0;
   initial_pose.pose.pose.position.y = 0.0;
   initial_pose.pose.pose.position.z = 0.0;

   tf2::Quaternion q;
   q.setRPY(0, 0, 0);
   initial_pose.pose.pose.orientation = tf2::toMsg(q);

   RCLCPP_INFO(this->get_logger(), "Publicando a pose inicial...");
   this->initial_pose_publisher_->publish(initial_pose);
};

void Vallet::subscription_callback(const geometry_msgs::msg::Pose::SharedPtr msg)
{
   auto goal_pose = std::make_unique<geometry_msgs::msg::PoseStamped>(this->create_pose_stamped(msg->position.x, msg->position.y, 0.0f));

   RCLCPP_INFO(this->get_logger(), "Adicionando um ponto a fila...");
   this->poses_to_go_.push_back(std::move(goal_pose));
};

geometry_msgs::msg::PoseStamped Vallet::create_pose_stamped(float pos_x, float pos_y, float rot_z)
{
   tf2::Quaternion q;
   q.setRPY(0, 0, rot_z);

   geometry_msgs::msg::PoseStamped pose;
   pose.header.frame_id = "map";
   pose.header.stamp = this->get_clock()->now();
   pose.pose.position.x = pos_x;
   pose.pose.position.y = pos_y;
   pose.pose.position.z = 0; // Geralmente 0 para robôs terrestres
   pose.pose.orientation.x = q.x();
   pose.pose.orientation.y = q.y();
   pose.pose.orientation.z = q.z();
   pose.pose.orientation.w = q.w();

   return pose;
};

void Vallet::timer_callback()
{
   if (this->poses_to_go_.size() > 0 && !this->ocuppied_)
   {
      RCLCPP_INFO(this->get_logger(), "Fila não está vazia, mandando trabalho...");
      this->send_goal(*poses_to_go_[0]);
   }
   else
      return;
};

void Vallet::send_goal(const geometry_msgs::msg::PoseStamped &goal_pose)
{
   if (!action_client_nav2_->wait_for_action_server(std::chrono::seconds(10)))
   {
      RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
      return;
   }

   auto goal = nav2_msgs::action::NavigateToPose::Goal();
   goal.pose = goal_pose;

   auto send_goal_options = rclcpp_action::Client<nav2_msgs::action::NavigateToPose>::SendGoalOptions();
   send_goal_options.result_callback = std::bind(&Vallet::result_send_goal_callback, this, std::placeholders::_1);

   this->ocuppied_ = true;

   action_client_nav2_->async_send_goal(goal, send_goal_options);
};

void Vallet::result_send_goal_callback(const rclcpp_action::ClientGoalHandle<nav2_msgs::action::NavigateToPose>::WrappedResult &result)
{
   switch (result.code)
   {
   case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_INFO(this->get_logger(), "Goal was SUCCEEDED!");
      this->poses_to_go_.erase(this->poses_to_go_.begin());
      RCLCPP_INFO(this->get_logger(), "Pose removed.");
      this->ocuppied_ = false;
      return;
   case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_ERROR(this->get_logger(), "Goal was aborted");
      this->ocuppied_ = false;
      return;
   case rclcpp_action::ResultCode::CANCELED:
      RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
      this->ocuppied_ = false;
      return;
   default:
      RCLCPP_ERROR(this->get_logger(), "Unknown result code");
      this->ocuppied_ = false;
      return;
   }
};