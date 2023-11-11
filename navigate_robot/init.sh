#!/bin/zsh

# Inicializa o ambiente ROS 2
source /opt/ros/humble/setup.zsh

# Compila o pacote desejado
colcon build --packages-select navigate_robot

# Inicializa o ambiente ROS 2
source ./install/setup.zsh

ros2 launch navigate_robot navigate_robot.xml