// iot-devices/environment_sensor.cpp
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 2  // Pin connected to DHT sensor
#define DHTTYPE DHT11  // DHT 11

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";
const char* mqtt_server = "mqtt.example.com";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void setup_wifi() {
  delay(10);
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read sensor data
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    // Create a JSON payload
    String payload = "{\"temperature\":" + String(temp) + ", \"humidity\":" + String(hum) + "}";
    client.publish("environment/sensor", payload.c_str());
  }

  delay(5000);  // Send data every 5 seconds
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      delay(5000);
    }
  }
}
