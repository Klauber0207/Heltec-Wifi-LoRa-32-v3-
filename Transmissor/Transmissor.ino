TCC: Sistema de monitoramento utilizando Heltec wifi lora 32 v3
Autor: Klauber Araujo Sousa
Ano: 2023



#include <Arduino.h>
#include <LoRa.h>

const int sensorPin = 48; // Pino do sensor de movimento (OUT)
int lastDetectionTime = 0;
int detectionInterval = 30000; // Intervalo de detecção (30 segundos)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!LoRa.begin(915E6)) {
    Serial.println("Erro ao iniciar módulo LoRa.");
    while (1);
  }
  Serial.println("Módulo LoRa iniciado.");

  // Configura o pino do sensor como entrada
  pinMode(sensorPin, INPUT);
}

void loop() {
  int sensorValue = digitalRead(sensorPin);

  if (sensorValue == HIGH) {
    // Movimento detectado
    int currentTime = millis();

    if (currentTime - lastDetectionTime >= detectionInterval) {
      sendMessageToBase("Atividade suspeita detectada!");
      lastDetectionTime = currentTime;
    }
  }
}

void sendMessageToBase(String message) {
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
  Serial.println("Mensagem enviada para a Estação Base.");
  delay(10000); // Aguarda 10 segundos para evitar múltiplos envios.
}
