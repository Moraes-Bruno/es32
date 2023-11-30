#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Quarto_2";
const char* password = "1234awsd";
const char* serverUrl = "http://192.168.1.104:3000/atualizar-vagas"; // Update with your server address

const byte trigPins[] = {18, 17, 2, 25}; // Update with your actual pin numbers
const byte echoPins[] = {5, 16, 15, 26};    // Update with your actual pin numbers

byte vagasOcupadas[4] = {0};

void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; ++i) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi.");
}

void loop() {
  acionamento();
  enviarStatusVagas(); // Add this line to send the status of all parking spaces to the server
}

void acionamento() {
  for (int i = 0; i < 4; ++i) {
    // Ultrasonic sensor logic (similar to your existing code)
    digitalWrite(trigPins[i], LOW);
    delayMicroseconds(2);
    digitalWrite(trigPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPins[i], LOW);
    long duration = pulseIn(echoPins[i], HIGH);
    float distance_cm = duration * 0.034 / 2;

    // Update the occupied status
    vagasOcupadas[i] = (distance_cm <= 10) ? 1 : 0;

    // Display distance and occupied status in the serial monitor
    Serial.print("Vaga ");
    Serial.print(i);
    Serial.print(" - Distância: ");
    Serial.print(distance_cm);
    Serial.println(" cm");
    Serial.print("Vaga Ocupada? ");
    Serial.println(vagasOcupadas[i]);

    // Short delay before the next sensor reading
    delay(500);
  }
}

void enviarStatusVagas() {
  HTTPClient http;

  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");

  String jsonData = "{\"vagasOcupadas\": [" +
                    String(vagasOcupadas[0]) + "," +
                    String(vagasOcupadas[1]) + "," +
                    String(vagasOcupadas[2]) + "," +
                    String(vagasOcupadas[3]) + "]}";

  int httpCode = http.POST(jsonData);


  if (httpCode == 200) {
    Serial.println("Status das vagas enviado com sucesso");
  } else {
    Serial.println("Falha ao enviar o status das vagas");
  }

  http.end();
  delay(2000); // Wait for a while before sending again (adjust as needed)
}
