# Objetivo
O objetivo desse repositório é armazenar os códigos para o seguinte exercício:
Desenvolva um pacote em ROS com as funcionalidades de mapeamento e navegação utilizando o turtlebot 3 (simulado ou real).

## Arquivos
*Para rodar esse projeto, é necessário ter o ros2 humble e o xterm instalado e devidamente funcionando*.
Cada pasta, `mapping_launch` e `navigate_robot` são cada um pacotes ros2 independentes. Lembre-se que é necessário já estar com a simulação no gazebo rodando e totalmente configurada para o turtlebot3 model burger.
### Rodar o mapeamento
Na root do pacote digite:
```bash
chmod +x mapping_zsh.sh
```
```bash
./mapping_zsh.sh
```
Esse aquivo `.sh` funciona APENAS EM TERMINAIS ZSH. Ele basicamente compila o pacote, instala o pacote no terminal, lança em um novo terminal xterm o pacote criado, o `mapping_launch`, e no terminal original espera pelo comando "y" para salvar o mapa. O mapa salvo esta localizado em `/assets` na pasta do pacote (`map` para mapeamento com o cartographer e `slam_map` para mapeamento com o slam).

### Rodar a navegação
Na root do pacote digite:
```bash
chmod +x init.sh
```
```bash
./init.sh
```
Esse aquivo `.sh` funciona APENAS EM TERMINAIS ZSH. Ele basicamente compila o pacote, instala o pacote no terminal e lança o pacote criado, o `navigate_robot`. O mapa que ele utiliza esta localizado na pasta `assets/map` na pasta do pacote.

# Mapeamento
O mapeamento foi feito de duas maneiras:
- Com o cartographer, o qual gera um mapa que é necessário informar o "initialpose" do robô nele.
- Com o SLAM, uma maneira que é gerado um mapa em que o robô "se encontra" nele.
## Usando Cartographer
### Vídeo do funcionamento
https://github.com/Lemos1347/inteli-modulo-8-ponderada-2/assets/99190347/5efbffb2-a952-4466-81d5-13c77568b430

## Usando SLAM
### Vídeo do funcionamento
https://github.com/Lemos1347/inteli-modulo-8-ponderada-2/assets/99190347/ad3fbbaf-52cb-4d4e-8955-8db95732ebb0

# Navegação
Para a navegação, foi utilizado o SLAM, para que o sistema seja *idempotente*.
## Video do funcionamento 
https://github.com/Lemos1347/inteli-modulo-8-ponderada-2/assets/99190347/f1d95cca-feb9-429c-b60a-d8e0b285a2c4
