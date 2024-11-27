#include <ESP8266WiFi.h>  // Use this for NodeMCU (ESP8266)
#include <WiFiManager.h>
#include <LittleFS.h>      // Use LittleFS instead of SPIFFS for ESP8266
#include <ArduinoJson.h>

// WiFiManager custom parameters
const char* ssid = "839";             // Replace with your default WiFi SSID
const char* password = "8399009050";  // Replace with your default WiFi password

char mqtt_server[20] = "broker.hivemq.com";  // Default MQTT server
char port[6] = "1883";                       // Default MQTT port
char topic[40] = "AirQualityData";           // Default MQTT topic

bool shouldSaveConfig = false;

// MQ135 Sensor Pin
#define MQ135_PIN A0  // Analog pin connected to MQ135

// Callback to save config if necessary
void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

// Function to initialize WiFi connection and load/save configurations
void setup_wifi() {
  Serial.println("Mounting FS...");
  
  if (LittleFS.begin()) {  // Initialize LittleFS
    Serial.println("Mounted file system");

    if (LittleFS.exists("/config.json")) {  // Check if the config file exists
      Serial.println("Reading config file...");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);

        StaticJsonDocument<400> jsonDoc;
        DeserializationError error = deserializeJson(jsonDoc, buf.get());
        if (!error) {
          // If JSON is valid, load values into the variables
          strcpy(mqtt_server, jsonDoc["mqtt_server"]);
          strcpy(port, jsonDoc["port"]);
          strcpy(topic, jsonDoc["topic"]);
        } else {
          Serial.println("Failed to parse config file");
        }
      }
    }
  } else {
    Serial.println("Failed to mount file system");
  }

  // WiFiManager setup with custom parameters
  WiFiManagerParameter custom_mqtt_server("mqtt_server", "MQTT Server", mqtt_server, 40);
  WiFiManagerParameter custom_port("port", "Port", port, 6);
  WiFiManagerParameter custom_topic("topic", "Topic", topic, 40);

  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  
  // Add custom parameters to the WiFiManager portal
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_port);
  wifiManager.addParameter(&custom_topic);

  // Start the WiFiManager portal
  if (!wifiManager.autoConnect("NodeMCU_AirQuality")) {
    Serial.println("Failed to connect and configure WiFi.");
    ESP.restart(); // Restart if WiFi fails to connect
  }

  // After successful connection, save custom parameters to LittleFS if necessary
  if (shouldSaveConfig) {
    StaticJsonDocument<400> jsonDoc;
    jsonDoc["mqtt_server"] = mqtt_server;
    jsonDoc["port"] = port;
    jsonDoc["topic"] = topic;

    File configFile = LittleFS.open("/config.json", "w");
    if (configFile) {
      serializeJson(jsonDoc, configFile);
      configFile.close();
      Serial.println("Saved config");
    } else {
      Serial.println("Failed to open config file for writing");
    }
  }

  Serial.println("Connected to WiFi.");
}

void setup() {
  Serial.begin(115200);

  // Initialize WiFi and load/save configurations
  setup_wifi();

  // Initialize the sensor pin
  pinMode(MQ135_PIN, INPUT);
}

void loop() {
  // Read data from the MQ135 sensor
  int airQualityValue = analogRead(MQ135_PIN);

  // Convert the analog value to a readable format (example: PPM or voltage)
  float voltage = (airQualityValue / 1023.0) * 5.0;  // Assuming a 5V reference

  // Create a JSON payload for sending data
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["AirQualityValue"] = airQualityValue;
  jsonDoc["Voltage"] = voltage;

  // Serialize JSON for MQTT
  char jsonBuffer[200];
  serializeJson(jsonDoc, jsonBuffer);

  // Display the air quality values on the Serial Monitor
  Serial.print("Air Quality Value: ");
  Serial.println(airQualityValue);
  Serial.print("Voltage: ");
  Serial.println(voltage);

  // Here you can add MQTT publishing logic to send data to the server
  // Example:
  // mqttClient.publish(topic, jsonBuffer);

  delay(2000);  // Read sensor data every 2 seconds
}
