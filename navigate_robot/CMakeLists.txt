cmake_minimum_required(VERSION 3.8)
project(navigate_robot)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav2_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(slam_toolbox REQUIRED)

# -----------------> TESTE <-----------------
# add_executable(navigate_robot src/main.cpp)
# ament_target_dependencies(navigate_robot rclcpp geometry_msgs nav2_msgs tf2 tf2_geometry_msgs tf2_ros)
# install(TARGETS navigate_robot DESTINATION lib/${PROJECT_NAME}/)
# -----------------> TESTE <-----------------

# -----------------> move_robot <-----------------
add_executable(move_robot src/move_robot/main.cpp src/move_robot/vallet/vallet.cpp)

ament_target_dependencies(move_robot rclcpp geometry_msgs nav2_msgs tf2 tf2_geometry_msgs tf2_ros slam_toolbox)

install(TARGETS move_robot DESTINATION lib/${PROJECT_NAME}/)
# -----------------> move_robot <-----------------

# -----------------> talk_to_robot <-----------------
add_executable(talk_to_robot src/talk_to_robot/main.cpp src/talk_to_robot/talker/talker.cpp)

ament_target_dependencies(talk_to_robot rclcpp geometry_msgs nav2_msgs tf2 tf2_geometry_msgs tf2_ros)

install(TARGETS talk_to_robot DESTINATION lib/${PROJECT_NAME}/)
# -----------------> talk_to_robot <-----------------

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
