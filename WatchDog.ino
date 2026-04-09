'
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Wi-Fi
const char* ssid = "katatonia";
const char* password = "Thegreatcolddistance";

// URL de teste
const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

// LEDs
const int LED_VERDE = 18;
const int LED_VERMELHO = 19;

// Tempo de checagem (15s)
const unsigned long intervalo = 15000;
unsigned long ultimoTempo = 0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH); 

  Serial.println("Conectando ao Wi-Fi...");
  WiFi.begin(ssid, password);
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi desconectado! Tentando reconectar...");
    
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    delay(5000);
    return;
  }

  if (millis() - ultimoTempo >= intervalo) {
    ultimoTempo = millis();

    Serial.println("\n[CHECK] Verificando servidor...");

    WiFiClientSecure *client = new WiFiClientSecure;

    if (client) {
      client->setInsecure(); 
      HTTPClient https;

      if (https.begin(*client, serverName)) {

        int httpCode = https.GET();

        if (httpCode > 0) {
          Serial.printf("Código HTTP: %d\n", httpCode);

          if (httpCode == HTTP_CODE_OK) {
            Serial.println("Servidor funcionando");

            digitalWrite(LED_VERDE, HIGH);
            digitalWrite(LED_VERMELHO, LOW);

          } else {
            Serial.println("Erro no servior");

            digitalWrite(LED_VERDE, LOW);
            digitalWrite(LED_VERMELHO, HIGH);
          }

        } else {
          Serial.printf("Falha na requisição: %s\n",
                        https.errorToString(httpCode).c_str());

          digitalWrite(LED_VERDE, LOW);
          digitalWrite(LED_VERMELHO, HIGH);
        }

        https.end();

      } else {
        Serial.println("Não conseguiu conectar ao servidor");

        digitalWrite(LED_VERDE, LOW);
        digitalWrite(LED_VERMELHO, HIGH);
      }

      delete client; 
    }
  }
}

'
