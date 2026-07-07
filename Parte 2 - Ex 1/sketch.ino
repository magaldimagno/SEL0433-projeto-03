// Alexandre Augusto Novarino Britto - 14754672
// Eduardo Magaldi Magno - 15448780
// Raphael Parra - 15554782

#include <ESP32Servo.h>

// Pinos de sinal
#define servopin  2
#define potenciometropin  4

// Inicializa o objeto Servo
Servo servo;

// Posição inicial
int pos = 0;

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  servo.setPeriodHertz(50);
  servo.attach(servopin, 500, 2400);
  pinMode(potenciometropin, INPUT);
}

void loop() {
  int potenciometro = analogRead(potenciometropin); 
  
  pos = map(potenciometro, 0, 4095, 0, 180); // Regra de três para determinar o valor do dutycycle do PWM.
  
  servo.write(pos);
  delay(15);
  Serial.print("Angulo Servo: ");
  Serial.println(pos);
}