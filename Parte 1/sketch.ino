// Pinos do LED RGB
#define REDpin 13
#define GREENpin 12
#define BLUEpin 14

// Parâmetros do PWM
const int frequencia = 5000;
const int resolucao = 8; 

// Variáveis para Duty Cycle e Incrementos distintos
int dutyRed = 0, dutyGreen = 0, dutyBlue = 0;
const int incGreen = 5;       
const int incBlue = 10;       
const int incRed = 15;        

void setup() {
  // Configuração Serial
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  // Associando pino, frequencia e resolucao
  ledcAttach(REDpin, frequencia, resolucao);
  ledcAttach(GREENpin, frequencia, resolucao);
  ledcAttach(BLUEpin, frequencia, resolucao);
}

void loop() {
  // Atualiza os Duty Cycles com incrementos diferentes
  dutyRed = (dutyRed + incRed) % 256;
  dutyGreen = (dutyGreen + incGreen) % 256;
  dutyBlue = (dutyBlue + incBlue) % 256;

  // Aplica o PWM nos canais
  ledcWrite(REDpin, dutyRed);
  ledcWrite(GREENpin, dutyGreen);
  ledcWrite(BLUEpin, dutyBlue);

  // Envio de mensagens via Serial
  Serial.print("Duty Cycles -> R: "); Serial.print(dutyRed);
  Serial.print(" (inc:"); Serial.print(incRed);
  Serial.print(") | G: "); Serial.print(dutyGreen);
  Serial.print(" (inc:"); Serial.print(incGreen);
  Serial.print(") | B: "); Serial.print(dutyBlue);
  Serial.print(" (inc:"); Serial.print(incBlue);
  Serial.println(")");

  delay(100);
}
