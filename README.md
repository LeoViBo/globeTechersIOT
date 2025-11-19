# Grupo GlobeTechers
Gabriel Augusto Maciel Toledo - RM: 551654
Leonardo Viotti Bonini - RM: 551716
Vinicius Santos Yamashita de Farias - RM: 550908

## Links
Video explicativo: https://www.youtube.com/watch?v=-RcAWtaCfFw
Projeto Wokwi: [https://wokwi.com/projects/447925872839534593](https://wokwi.com/projects/447925872839534593)

## Projeto
O nosso projeto de IOT busca melhorar o trabalho do dia a dia de uma pessoa que trabalha de casa, colocando um sensor que desenvolvemos com ESP32 dentro da casa d usuario que ira notificar se tem algo de errado em sua estação de trabalho ou nele durante o trabalho.  
A aplicação usa um potenciometro para checar os batimentos cardiacos do usuario que a aplicação avisaria para dar uma pausa caso estejam altos.   
Também com um potênciometro ele simula uma camêra que veria a postura do usuario e então avisaria se ele está com uma postura errada.  
E com um sensor de luz ele verifica se seu ambiente de trabalho não está muito escuro e que deve acender alguma luz.  
Todos os avisos são feitos por meio de um buzzer que notifica por som e um led para mostrar visualmente.

O projeto envia esses dados via MQTT com o hivemqt e o topico "gs2025/smartbreak/1" para que no futuro dseja desenvolvido uma aplicação que coletaria esses dados e mostraria pelo celular, que poderia usar o relgio inteligente para medir os batimentos cardiacos, ou para mostrar uma dashboard mostrando dados atravez dos dias.

O nosso objetivo com essa aplicação e avisar momentos de stress para a pessoa pausar um pouco o trabalho e também notificar caso estejam ruins as condições de trabalho para evitar problemas de coluna, fatiga de olhos e outros problemas de saude que podem vir a acontecer no trabalho home office.
