#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Pin definitions
const int trigPin = D1;  // Trig pin of the ultrasonic sensor
const int echoPin = D2;  // Echo pin of the ultrasonic sensor
const int ledPin = D5;   // LED pin

// Wi-Fi configuration
const char* ssid = "839";           // Replace with your Wi-Fi SSID
const char* password = "8399009050";    // Replace with your Wi-Fi password

// MQTT configuration
const char* mqttServer = "broker.hivemq.com"; // Public HiveMQ broker
const int mqttPort = 1883;                  // MQTT port (default)
const char* topic = "smartlight/led";             // MQTT topic

// Wi-Fi and MQTT client
WiFiClient espClient;
PubSubClient mqttClient(espClient);
void setup() {
    Serial.begin(115200);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(ledPin, OUTPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    // Set up MQTT server
    mqttClient.setServer(mqttServer, mqttPort);
}
void loop() {
    long duration, distance;

    // Trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echo pin
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034 / 2); // Distance in cm

    // Control LED and publish status
    if (distance < 30) { // Change distance threshold as needed
        digitalWrite(ledPin, HIGH);
        mqttClient.publish(topic, "ON");
    } else {
        digitalWrite(ledPin, LOW);
        mqttClient.publish(topic, "OFF");
    }
// Ensure MQTT client is connected
    if (!mqttClient.connected()) {
        if (mqttClient.connect("NodeMCUClient")) {
            Serial.println("Connected to MQTT");
        }
    }
    mqttClient.loop(); // Process MQTT messages
    delay(1000); // Adjust delay as necessary
}
