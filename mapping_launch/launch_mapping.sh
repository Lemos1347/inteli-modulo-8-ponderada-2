#!/bin/zsh

# Inicializa o ambiente ROS 2
source /opt/ros/humble/setup.zsh

# Compila o pacote desejado (opcional, dependendo do seu setup)
colcon build --packages-select mapping_launch

# Inicializa o ambiente ROS 2 (ajuste o caminho conforme necessário)
source ./install/setup.zsh

# Executa o comando ros2 launch em um novo terminal xterm
xterm -e "ros2 launch mapping_launch _launch.xml use_sim_time:=true" &
#xterm -hold -e 'bash -c "ros2 launch mapping_launch _launch.xml use_sim_time:=true & echo \$! > /tmp/ros_pid"' &
