#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <WiFi.h>
#include <UniversalTelegramBot.h>

#define RF_FREQUENCY            915000000 // Hz
#define TX_OUTPUT_POWER         14        // dBm
#define LORA_BANDWIDTH          0         // 125 kHz
#define LORA_SPREADING_FACTOR   7         // SF7
#define LORA_CODINGRATE         1         // 4/5
#define LORA_PREAMBLE_LENGTH    8
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON    false

#define BUFFER_SIZE             30

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

int16_t txNumber;
int16_t rssi, rxSize;
bool lora_idle = true;

// Configurações Wi-Fi
const char *ssid = "J TEC";
const char *password = "trompete1";

// Configurações do bot do Telegram
String BOT_TOKEN = "6306278032:AAFuoJfMRrIE12Bk0BwK9l87hCvvkRtxi70";
String CHAT_ID = "-1002081909315";

void setup() {
    Serial.begin(115200);
    Mcu.begin();

    // Inicialização do Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    txNumber = 0;
    rssi = 0;

    RadioEvents.RxDone = OnRxDone;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      0, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
}

void loop() {
    if (lora_idle) {
        lora_idle = false;
        Serial.println("into RX mode");
        Radio.Rx(0);
    }
    Radio.IrqProcess();
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    rssi = rssi;
    rxSize = size;
    memcpy(rxpacket, payload, size);
    rxpacket[size] = '\0';
    Radio.Sleep();

    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n", rxpacket, rssi, rxSize);

    // Extraindo as informações do sensor da string recebida
    int sensorValue;
    if (sscanf(rxpacket, "Sensor value: %d", &sensorValue) == 1) {
        Serial.printf("Sensor value: %d\r\n", sensorValue);

        // Verifica se o sensor detectou movimento
        if (sensorValue == 1) {
            sendTelegramMessage("Movimento detectado!");
        }
    } else {
        Serial.println("Failed to extract sensor value from the received packet.");
    }

    lora_idle = true;
}

// Função para enviar uma mensagem para o Telegram
void sendTelegramMessage(const char *message) {
    UniversalTelegramBot bot(BOT_TOKEN);
    bot.sendMessage(CHAT_ID, message, "");
}
