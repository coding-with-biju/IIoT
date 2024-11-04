#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Wi-Fi and ThingSpeak Configuration
const char* ssid = "839";            // Replace with your Wi-Fi SSID
const char* password = "8399009050";     // Replace with your Wi-Fi password
unsigned long channelID = 2726724;  // Replace with your ThingSpeak channel ID
const char* apiKey = "KDYAX0T1GC9F6CUH";  // Replace with your ThingSpeak Write API Key

WiFiClient client;

// DHT Sensor Setup
#define DHTPIN D2        
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE); 

void setup() {
    Serial.begin(9600);
    dht.begin();
    
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

    // Read temperature and humidity from DHT11 sensor
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    // Check if reading failed
    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT sensor!");
        return;  // Exit loop and try again
    }

    // Display on Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C ");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    // Send data to ThingSpeak
    ThingSpeak.setField(1, temp);  // Field 1: Temperature
    ThingSpeak.setField(2, hum);   // Field 2: Humidity
    int statusCode = ThingSpeak.writeFields(channelID, apiKey);

    if (statusCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully.");
    } else {
        Serial.print("Error sending data to ThingSpeak: ");
        Serial.println(statusCode);  // Display error code
    }
  
    delay(20000);  // ThingSpeak allows updates every 15 seconds; 20 seconds gives some buffer
}
