#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Wi-Fi and ThingSpeak Configuration
const char* ssid = "839";            // Replace with your Wi-Fi SSID
const char* password = "8399009050";     // Replace with your Wi-Fi password
unsigned long channelID = 2726724;  // Replace with your ThingSpeak channel ID
const char* apiKey = "KDYAX0T1GC9F6CUH";  // Replace with your ThingSpeak Write API Key

WiFiClient client;

// Ultrasonic Sensor Pin Definitions
const int trigPin = D1;
const int echoPin = D2;

void setup() {
    Serial.begin(9600);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    
    // Initialize ThingSpeak
    ThingSpeak.begin(client);
}

void loop() {
    // Check Wi-Fi connection and reconnect if needed
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected. Reconnecting...");
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.println("Connecting to WiFi...");
        }
        Serial.println("Reconnected to WiFi");
    }

    // Measure distance using ultrasonic sensor
    long duration, distance;

    // Trigger the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Read the echo pin
    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034) / 2;  // Calculate distance in cm

    // Display distance on Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Send distance data to ThingSpeak
    ThingSpeak.setField(1, distance);  // Field 1: Distance
    int statusCode = ThingSpeak.writeFields(channelID, apiKey);

    if (statusCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully.");
    } else {
        Serial.print("Error sending data to ThingSpeak: ");
        Serial.println(statusCode);  // Display error code
    }
  
    delay(20000);  // ThingSpeak allows updates every 15 seconds; 20 seconds gives some buffer
}
