# Projeto 3: Esteira Industrial Inteligente - Controle PWM e Comunicação
**Disciplina:** SEL0433 - Aplicação de Microprocessadores  
**Autores:**
* Alexandre Augusto Novarino Britto - 14754672
* Eduardo Magaldi Magno - 15448780
* Raphael Parra - 15554782

## Objetivo do Projeto
Este projeto implementa um sistema de controle para uma esteira industrial utilizando o microcontrolador **ESP32 DevKit**. O objetivo é exercitar a programação de periféricos de 32 bits, explorando o controle avançado de movimento via **MCPWM (Motor Control PWM)**, interface visual em **Display OLED via I2C**, monitoramento serial UART e tratamento de eventos em tempo real através de **interrupções externas**.

## Descrição do Sistema
O sistema simula a operação de uma esteira industrial onde um motor de passo (atuador) tem sua velocidade controlada pela frequência de pulsos PWM e seu sentido de rotação alternado instantaneamente por um botão físico. O status operacional é exibido em tempo real em um display OLED e reportado periodicamente via terminal serial para monitoramento.

## Especificações Técnicas e Implementação

### 1. Controle de Movimento com MCPWM
Utilizou-se a biblioteca nativa `driver/mcpwm.h`, projetada especificamente para controle de motores em hardware de 32 bits.
*   **Pino STEP (GPIO 19):** Recebe o sinal de clock gerado pelo periférico MCPWM. A velocidade de rotação é diretamente proporcional à frequência configurada.
*   **Pino DIR (GPIO 18):** Define a direção do motor através de níveis lógicos (HIGH para Horário, LOW para Anti-horário).

### 2. Interrupção Externa e Debounce
A inversão do sentido de rotação é gerenciada pelo **pino 4**, configurado com resistor de **pull-up interno** e uma interrupção por borda de descida (`FALLING`).
*   **Debounce por Software:** Para evitar múltiplos acionamentos devido ao ruído mecânico do botão, implementou-se uma lógica de debounce na ISR (`isrInverteSentido`) que ignora pulsos menores que 200ms usando a função `millis()`.

### 3. Interface Visual I2C (OLED SSD1306)
A exibição dos dados utiliza o barramento **I2C** nos pinos **SDA (GPIO 21)** e **SCL (GPIO 22)**.
*   O display apresenta a frequência operacional (Hz) e o sentido atual do movimento.
*   As bibliotecas `Adafruit_GFX` e `Adafruit_SSD1306` foram empregadas para o gerenciamento da matriz de pixels do display.

### 4. Comunicação Serial (UART)
O sistema mantém comunicação ativa a **115200 baud** para depuração.
*   Logs de monitoramento são enviados ao computador a cada 3 segundos, informando o estado do motor.
*   Mensagens instantâneas são disparadas via serial sempre que o sentido de rotação é alterado pelo usuário.



## Mapeamento de Hardware
| Componente       | Pino ESP32 | Função                                                  |
|------------------|------------|---------------------------------------------------------|
| Motor (STEP)     | GPIO 19    | Saída de Pulsos PWM que controlam a velocidade do motor |
| Motor (DIR)      | GPIO 18    | Controle de Direção                                     |
| Botão Inversão   | GPIO 4     | Interrupção Externa                                     |
| OLED SDA         | GPIO 21    | Dados I2C                                               |
| OLED SCL         | GPIO 22    | Clock I2C                                               |



## Como Executar
1.  Acesse o projeto no simulador **Wokwi**.
2.  No *Library Manager*, adicione as bibliotecas `Adafruit SSD1306` e `Adafruit GFX`.
3.  Inicie a simulação e abra o **Monitor Serial** (115200 baud).
4.  Interaja com o botão na GPIO 4 para inverter o sentido e observe as atualizações no OLED e no terminal.

## Evidência de funcionamento

[Vídeo de demonstração do projeto](https://youtu.be/PhbGobzU46I)


## Reflexão Técnica
A utilização da ESP32 neste projeto industrial justifica-se pela sua arquitetura de **32 bits** e clock de até **240 MHz**, que permitem o gerenciamento de periféricos complexos de forma simultânea. Diferente de microcontroladores de 8 bits, a ESP32 possui hardware dedicado para controle de motores (**MCPWM**), garantindo que a geração de pulsos de alta precisão não sofra atrasos causados pelo processamento da interface gráfica (OLED) ou da comunicação serial. Essa separação de tarefas em nível de hardware aumenta a robustez e a confiabilidade do sistema industrial embarcado.