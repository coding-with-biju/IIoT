#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Wi-Fi and ThingSpeak Configuration
const char* ssid = "839";            // Replace with your Wi-Fi SSID
const char* password = "8399009050";     // Replace with your Wi-Fi password
unsigned long channelID = 2726724;  // Replace with your ThingSpeak channel ID
const char* apiKey = "KDYAX0T1GC9F6CUH";  // Replace with your ThingSpeak Write API Key

WiFiClient client;

// LDR Sensor Pin Definition
const int ldrPin = A0;

void setup() {
    Serial.begin(9600);

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

    // Read light intensity data from LDR sensor
    int lightIntensity = analogRead(ldrPin);

    // Display light intensity on Serial Monitor
    Serial.print("Light Intensity (Analog Value): ");
    Serial.println(lightIntensity);

    // Send light intensity data to ThingSpeak
    ThingSpeak.setField(1, lightIntensity);  // Field 1: Light Intensity
    int statusCode = ThingSpeak.writeFields(channelID, apiKey);

    if (statusCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully.");
    } else {
        Serial.print("Error sending data to ThingSpeak: ");
        Serial.println(statusCode);  // Display error code
    }
  
    delay(20000);  // ThingSpeak allows updates every 15 seconds; 20 seconds gives some buffer
}
