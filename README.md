# Water-Tank-Monitor-STR

O projeto Water Tank Monitor é um trabalho feito para a disciplina de Sistemas em Tempo Real na Universidade de Brasília. 

Seu objetivo principal é implementar um sistema de monitoramento de reservatório de água com uma bomba de recalque usando sensores. O projeto deve usar um sistema em tempo real, implementando os conceitos de exclusão mútua (mutex) e prioridade entre tarefas, também foi medido o tempo de execução de cada tarefa a fim de se gerar um [relatório](./docs/lab_2.pdf).

Para o projeto foi utilizado o microcontrolador ESP32 devido à sua facilidade de acesso WiFi e integração com FreeRTOS. O framework do Arudino foi escolhido a fim de simplificar a implementação.

## Para dar build e upload

Para dar build e upload no código é necessário usar a extensão [PlatformIO](https://platformio.org/) do [Visual Studio Code](https://code.visualstudio.com/).




