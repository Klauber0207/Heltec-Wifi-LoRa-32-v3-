int sensorPin = 7; // Pino do sensor de presença
int leitura = 0; // Variável para armazenar a leitura do sensor
bool estadoSensor = false; // Variável para armazenar o estado do sensor

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  pinMode(sensorPin, INPUT); // Define sensorPin como entrada
}

void loop() {
  leitura = digitalRead(sensorPin); // Realiza a leitura do sensor de presença

  if (leitura == HIGH) { // Se leitura for igual a HIGH, o movimento foi detectado
    if (estadoSensor == false) { // Compara o valor de estado do sensor
      Serial.println("Movimento detectado"); // Imprime a mensagem no monitor serial
      estadoSensor = true; // Altera o valor de estadoSensor para true
    }
    delay(5000); // Tempo de espera antes de permitir mais detecções
  } else { // Se leitura for igual a LOW, não houve movimento detectado
    if (estadoSensor == true) { // Compara o valor de estado do sensor
      Serial.println("Nada Aqui"); // Imprime a mensagem no monitor serial
      estadoSensor = false; // Altera o valor de estadoSensor para false
    }
  }
}
