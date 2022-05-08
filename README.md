## Snake Game - MSP430F5529

Trabalho realizado na disciplina de laboratório de sistemas microprocessados

### Objetivos
Criar um programa, mais especificamente o jogo da cobrinha, no MSP430F5529 utilizando uma
matriz 8x8 para apresentar o jogo, um LCD para apresentar o estado do jogo e o módulo
Bluetooth para o controle do jogo.

### Materiais Utilizados
- MSP430F5529
- Módulo Matriz LED 8x8 com MAX7219
- LCD 16x2 - AZ/BR com Módulo I2C
- Módulo Bluetooth Serial HM-10 BLE 4.0

### Procedimentos
O MSP430F5529 será responsável por gerenciar os módulos utilizados e também será
responsável por gerenciar o estado do jogo.

Para comunicar os módulos com o MSP430F5529 foram feitas as seguintes conexões.

### Tipos de comunicação utilizados com cada módulo
- Matriz LED 8x8 ➜ SPI
- LCD 16x2 ➜ I2C
- Bluetooth ➜ UART

### Fluxo do programa

### Organização do programa

### Funcionamento do programa

Foi utilizado o programa [Serial Bluetooth Terminal](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal) para enviar os comandos via Bluetooth.

Os comandos que podem ser enviados pelo Bluetooth são apresentados abaixo (em ASCII):
- “0” ➜ Muda a direção da cobra para cima.
- “1” ➜ Muda a direção da cobra para a direita
- “2” ➜ Muda a direção da cobra para baixo.
- “3” ➜ Muda a direção da cobra para a esquerda.
- “4” ➜ Pausa o jogo.
- “5” ➜ Reinicia o jogo.

Conforme são enviados os comandos o programa os processa e mostra o estado atual na
matriz e no LCD.

### Demonstração do funcionamento do projeto final
