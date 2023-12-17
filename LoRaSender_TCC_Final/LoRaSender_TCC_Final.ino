TCC: Sistema de monitoramento utilizando Heltec wifi lora 32 v3
Autor: Klauber Araujo Sousa
Ano: 2023


#include "LoRaWan_APP.h"
#include "Arduino.h"

#define RF_FREQUENCY            915000000 // Hz
#define TX_OUTPUT_POWER         5         // dBm
#define LORA_BANDWIDTH          0         // 125 kHz
#define LORA_SPREADING_FACTOR   7         // SF7
#define LORA_CODINGRATE         1         // 4/5
#define LORA_PREAMBLE_LENGTH    8
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON    false

#define RX_TIMEOUT_VALUE        1000
#define BUFFER_SIZE             30

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
bool lora_idle = true;
int16_t txNumber;

static RadioEvents_t RadioEvents;

// Define o pino ao qual o sensor está conectado
const int pirSensorPin = 7; // Assumindo que o sensor está conectado ao pino D4

void setup() {
    Serial.begin(115200);
    Mcu.begin();

    txNumber = 0;

    RadioEvents.TxDone = OnTxDone;
    Radio.Init(&RadioEvents);
    Radio.SetChannel(RF_FREQUENCY);
    Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                      LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                      LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                      true, 0, 0, LORA_IQ_INVERSION_ON, 3000);

    // Inicializa o pino do sensor como entrada
    pinMode(pirSensorPin, INPUT);
}

void loop() {
    if (lora_idle == true) {
        delay(5000);
        txNumber += 0.01;

        // Leitura do sensor
        int sensorValue = digitalRead(pirSensorPin);

        // Coloca os dados do sensor na variável txpacket
        sprintf(txpacket, "Sensor value: %d, Hello world number %0.2f", sensorValue, txNumber);

        Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));
        Radio.Send((uint8_t *)txpacket, strlen(txpacket));
        lora_idle = false;
    }
    Radio.IrqProcess();
}

void OnTxDone(void) {
    Serial.println("TX done......");
    lora_idle = true;
}
