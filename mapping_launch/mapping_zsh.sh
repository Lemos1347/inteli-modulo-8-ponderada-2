#!/bin/zsh

# Função para iniciar o processo de lançamento do ROS
launch_ros() {
  ./launch_mapping.sh true &
  ros_pid=$!
  sleep 2
  echo "ROS launch started with PID: $ros_pid"
}

# Função para salvar o mapa
save_map() {
  echo "Salvando o mapa..."
  ros2 run nav2_map_server map_saver_cli -f ./assets/map/map
}

# Inicia o processo ROS
launch_ros

echo "Pressione Y para salvar o mapa e encerrar o processo." 

# Aguarda o usuário pressionar CTRL+C
#echo "Pressione Y para salvar o mapa e encerrar o processo."

# Mantém o script em execução até receber o sinal de CTRL+C
while true; do
    echo -n "Pressione 'y' para salvar o mapa e sair: "
    read input
    if [ "$input" = "y" ]; then
        save_map
        break
    fi
done

wait $ros_pid