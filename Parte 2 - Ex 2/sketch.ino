// Alexandre Augusto Novarino Britto - 14754672
// Eduardo Magaldi Magno - 15448780
// Raphael Parra - 15554782

#include <Adafruit_SSD1306.h>
#include "driver/mcpwm.h"

// Definições de Hardware
#define PIN_STEP 19      
#define PIN_DIR 18       
#define PIN_BTN_INV 4    
#define OLED_SDA 21
#define OLED_SCL 22

// Variáveis de Controle
volatile bool sentidoHorario = true;
volatile bool atualizarInterface = true; // Flag para atualizar display/serial só quando necessário
int frequenciaPasso = 0; 

Adafruit_SSD1306 display(128, 64, &Wire, -1);

// Interrupção com Debounce via Software
void IRAM_ATTR isrInverteSentido() {
  static unsigned long ultimoTempo = 0;
  unsigned long tempoAtual = millis();
  
  // Ignora ruídos menores que 200ms
  if (tempoAtual - ultimoTempo > 200) {
    sentidoHorario = !sentidoHorario;
    atualizarInterface = true; 
    ultimoTempo = tempoAtual;
    Serial.print("Novo Sentido: ");
    Serial.println(sentidoHorario ? "HORÁRIO" : "ANTI-HORÁRIO");
  }
}

// Função isolada para gerenciar o display sem travar o loop
void atualizaOLED() {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("SISTEMA INDUSTRIAL");
  display.print("Freq: "); display.print(frequenciaPasso); display.println(" Hz");
  display.print("Sentido: "); display.println(sentidoHorario ? "HOR" : "ANTI");
  display.display();
}

void setup() {
  Serial.begin(115200);

  // Configuração dos Pinos
  pinMode(PIN_DIR, OUTPUT);
  digitalWrite(PIN_DIR, sentidoHorario ? HIGH : LOW);
  
  pinMode(PIN_BTN_INV, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_INV), isrInverteSentido, FALLING);

  // Inicialização do OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erro no OLED");
  }
  display.setTextColor(WHITE);
  atualizaOLED();

  // Inicialização do MCPWM
  mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, PIN_STEP);
  mcpwm_config_t pwm_config;
  pwm_config.frequency = 1000; 
  pwm_config.cmpr_a = 0;       
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
  mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0); 
}

void loop() {
  // Processamento da Interação Serial
  if (Serial.available() > 0) {
    char comando = Serial.read();
    
    if (comando == '\n' || comando == '\r') return; 

    if (comando == '0') {
      frequenciaPasso = 0;
      mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A); 
      Serial.println("[Serial] Parar (0 Hz)");
      atualizarInterface = true;
    } 
    else if (comando == '1') {
      frequenciaPasso = 500;
      mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, frequenciaPasso);
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 50.0);
      mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
      Serial.println("[Serial] Baixa (500 Hz)");
      atualizarInterface = true;
    } 
    else if (comando == '2') {
      frequenciaPasso = 1500;
      mcpwm_set_frequency(MCPWM_UNIT_0, MCPWM_TIMER_0, frequenciaPasso);
      mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 50.0);
      mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
      Serial.println("[Serial] Alta (1500 Hz)");
      atualizarInterface = true;
    }
  }

  // Atualização Física do Sentido
  digitalWrite(PIN_DIR, sentidoHorario ? HIGH : LOW);

  // Atualização Condicional de Interface
  if (atualizarInterface) {
    atualizaOLED();
    atualizarInterface = false;
  }

  // Monitoramento Periódico
  static unsigned long ultimoLog = 0;
  if (millis() - ultimoLog >= 3000) {
    Serial.print("[Monitoramento] Motor: ");
    Serial.print(frequenciaPasso);
    Serial.print(" Hz | Sentido: ");
    Serial.println(sentidoHorario ? "HORÁRIO" : "ANTI-HORÁRIO");
    ultimoLog = millis();
  }
}