#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "SUA_REDE_WIFI";
const char* password = "SUA_SENHA_WIFI";
const char* serverUrl = "http://seu-servidor:3000/atualizar-vaga"; // Substitua com o endereço do seu servidor

const byte trigPin = 18;
const byte echoPin = 5;
const byte ledVermelho = 2;
const byte ledVerde = 15;

byte vagaOcupada = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi.");
}

void loop() {
  acionamento();
  enviarStatusVaga(); // Adicione esta linha para enviar o status da vaga ao servidor
}

void acionamento(){
  // Dispara o pulso de ultrassom
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mede o tempo de ida e volta do pulso
  long duration = pulseIn(echoPin, HIGH);

  // Calcula a distância em centímetros
  float distance_cm = duration * 0.034 / 2;

  if(distance_cm <= 200){
    vagaOcupada = 1;
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledVermelho, HIGH);
  }else{
    vagaOcupada = 0;
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledVerde, HIGH);
  }

  // Exibe a distância no monitor serial
  Serial.print("Distância: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  Serial.print("Vaga Ocupada? ");
  Serial.println(vagaOcupada);

  // Aguarde um curto intervalo antes de fazer a próxima leitura
  delay(500);
}

void enviarStatusVaga() {
  HTTPClient http;

  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  // Crie um JSON com o status da vaga
  String jsonData = "{\"vagaOcupada\": " + String(vagaOcupada) + "}";

  int httpCode = http.POST(jsonData);

  if (httpCode == 200) {
    Serial.println("Status da vaga enviado com sucesso");
  } else {
    Serial.println("Falha ao enviar o status da vaga");
  }

  http.end();
  delay(5000); // Aguarde um intervalo antes de enviar novamente (ajuste conforme necessário)
}
