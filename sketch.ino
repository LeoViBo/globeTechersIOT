#include <WiFi.h>
#include <PubSubClient.h>

// Config
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "gs2025/smartbreak/1";

// Pinos
const int POT_BPM_PIN = 34;     // potenciometro simula batimentos
const int POT_POST_PIN = 35;    // potenciometro simula postura
const int LDR_PIN = 32;         // LDR capta luz do ambiente
const int LED_PIN = 25;         // luz que ativa quando tem algo de errado
const int BUZZER_PIN = 26;      // som que toca quando tem algo de errado

// Treshholds de alertas
const float BPM_ALERT = 0.7;    // se batimentos altos -> alerta
const float POSTURE_BAD = 0.6;  // se postura ruim -> alerta
const float LIGHT_LOW = 0.5;    // luz muito baixa -> alerta

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 2000;

// Conecta com Wifi
void setup_wifi() {
  if (strlen(ssid) == 0) {
    Serial.println("SSID vazio, pulando conexão WiFi (modo standalone)");
    return;
  }
  Serial.print("Conectando WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 30) {
    delay(500);
    Serial.print(".");
    tries++;
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi conectado, IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Falha WiFi (continuando sem rede)");
  }
}

// Conecta com MQTT
void mqttReconnect() {
  if (strlen(ssid) == 0) return;
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32SmartBreak_noDHT")) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando em 2s");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

// Lê o valor analógico e converte para um número entre 0.0 e 1.0 para facilitar comparações para os alertas
float readNormalizedADC(int pin) {
  int v = analogRead(pin);
  return (float)v / 4095.0;
}


// Liga alerta
void alertOn(bool on) {
  digitalWrite(LED_PIN, on ? HIGH : LOW);
  if (on) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// Publicar MQTT
void publishState(float bpmNorm, float postureNorm, float lightNorm) {
  char payload[200];
  snprintf(payload, sizeof(payload),
           "{\"bpm\":%.3f,\"postura\":%.3f,\"luz\":%.3f}",
           bpmNorm, postureNorm, lightNorm);
  Serial.print("JSON -> ");
  Serial.println(payload);
  if (WiFi.status() == WL_CONNECTED && client.connected()) {
    client.publish(mqtt_topic, payload);
  } else if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    mqttReconnect();
    if (client.connected()) client.publish(mqtt_topic, payload);
  }
}

// Funcionamento do programa
void loop() {
  if (strlen(ssid) > 0 && WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) mqttReconnect();
    client.loop();
  }

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL) {
    lastSend = now;

    float bpm = readNormalizedADC(POT_BPM_PIN);
    float posture = readNormalizedADC(POT_POST_PIN);
    float light = readNormalizedADC(LDR_PIN);

    bool needPause = false;
    if (bpm > BPM_ALERT) needPause = true;
    if (posture > POSTURE_BAD) needPause = true;
    if (light > LIGHT_LOW) needPause = true;

    publishState(bpm, posture, light);
    alertOn(needPause);

    //Print para comparação com MQTT
    Serial.printf("bpm=%.3f postura=%.3f luz=%.3f ajustar/pausar=%d\n",
                  bpm, posture, light, needPause ? 1 : 0);
  }
}
