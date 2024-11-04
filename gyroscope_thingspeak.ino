#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// Wi-Fi and ThingSpeak Configuration
const char* ssid = "839";            // Replace with your Wi-Fi SSID
const char* password = "8399009050";     // Replace with your Wi-Fi password
unsigned long channelID = 2726724;  // Replace with your ThingSpeak channel ID
const char* apiKey = "KDYAX0T1GC9F6CUH";  // Replace with your ThingSpeak Write API Key

WiFiClient client;

// MPU6050 I2C address
const int MPU_ADDR = 0x68;

// Variables to store accelerometer data
int16_t accX, accY, accZ;

void setup() {
    Serial.begin(9600);
    Wire.begin(D2, D1);  // SDA = D2, SCL = D1

    // Initialize MPU6050
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  // Access power management register
    Wire.write(0);     // Wake up the MPU6050
    Wire.endTransmission(true);

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

    // Request accelerometer data from MPU6050
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // Starting register for accelerometer data
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 6, true);

    // Read accelerometer data
    accX = Wire.read() << 8 | Wire.read();
    accY = Wire.read() << 8 | Wire.read();
    accZ = Wire.read() << 8 | Wire.read();

    // Display data on Serial Monitor
    Serial.print("Acceleration X: "); Serial.print(accX);
    Serial.print(" | Y: "); Serial.print(accY);
    Serial.print(" | Z: "); Serial.println(accZ);

    // Send data to ThingSpeak
    ThingSpeak.setField(1, accX);  // Field 1: X-axis acceleration
    ThingSpeak.setField(2, accY);  // Field 2: Y-axis acceleration
    ThingSpeak.setField(3, accZ);  // Field 3: Z-axis acceleration
    int statusCode = ThingSpeak.writeFields(channelID, apiKey);

    if (statusCode == 200) {
        Serial.println("Data sent to ThingSpeak successfully.");
    } else {
        Serial.print("Error sending data to ThingSpeak: ");
        Serial.println(statusCode);  // Display error code
    }

    delay(20000);  // ThingSpeak allows updates every 15 seconds; 20 seconds gives some buffer
}
