#include <Arduino.h>
#include <LoRa.h>
#include <UniversalTelegramBot.h>

#define LORA_SS 18
#define LORA_RST 14
#define LORA_DI0 26

#define TELEGRAM_BOT_TOKEN "6306278032:AAFuoJfMRrIE12Bk0BwK9l87hCvvkRtxi70"
#define TELEGRAM_CHAT_ID "-1002081909315"

UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Iniciando módulo LoRa...");
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Erro ao iniciar módulo LoRa.");
    while (1);
  }c
  Serial.println("Módulo LoRa iniciado.");

  LoRa.onReceive(onReceive);
  LoRa.receive();

  Serial.println("Aguardando mensagens do sensor Heltec WiFi LoRa 32 V3...");
}

void loop() {
  // A estação base apenas aguarda as mensagens recebidas.
}

void onReceive(int packetSize) {
  if (packetSize > 0) {
    String receivedMessage = "";

    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }

    Serial.print("Mensagem recebida: ");
    Serial.println(receivedMessage);

    if (receivedMessage.indexOf("Atividade suspeita detectada!") != -1) {
      receivedMessage = "Atenção: Atividade suspeita detectada no local!";
      sendTelegramMessage(receivedMessage);
    }
  }
}

void sendTelegramMessage(String message) {
  if (bot.sendMessage(TELEGRAM_CHAT_ID, message, "")) {
    Serial.println("Mensagem enviada para o Telegram com sucesso!");
  } else {
    Serial.println("Erro ao enviar mensagem para o Telegram.");
  }
}
