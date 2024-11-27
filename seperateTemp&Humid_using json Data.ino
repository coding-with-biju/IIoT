#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>  

const char* ssid = "839";            // Replace with your Wi-Fi SSID
const char* password = "8399009050"; // Replace with your Wi-Fi password

const char* mqtt_server = "broker.hivemq.com"; // Replace with your MQTT broker
const int mqtt_port = 1883;
const char* mqtt_topic = "dht11/temhum"; // Topic to publish data

WiFiClient espClient;
PubSubClient client(espClient);  // MQTT client

// DHT Sensor Setup
#define DHTPIN D4        
#define DHTTYPE DHT11    

DHT dht(DHTPIN, DHTTYPE); 

// Initialize variables
float temp, hum;
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (200)
char json[MSG_BUFFER_SIZE];  // Buffer for the JSON message

void setup() {
    Serial.begin(9600);
    dht.begin();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqttCallback);
}

void reconnect() {
    // Loop until we're reconnected to MQTT broker
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Handle incoming messages if needed (for this case it's not used)
}

void loop() {
    // Reconnect to MQTT if disconnected
    if (!client.connected()) {
        reconnect();
    }
    client.loop();  // Maintain MQTT connection

    // Read temperature and humidity from DHT11 sensor
    temp = dht.readTemperature();
    hum = dht.readHumidity();

    // Check if the reading failed
    if (isnan(temp) || isnan(hum)) {
        Serial.println("Failed to read from DHT sensor!");
        return;  // Exit loop and try again
    }

    // Prepare JSON document
    StaticJsonDocument<200> doc;
    JsonObject root = doc.to<JsonObject>();
    root["Temp"] = temp;
    root["Humid"] = hum;

    // Serialize JSON to string
    serializeJson(root, json);

    // Display data on Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print(" °C ");
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.println(" %");

    // Publish the JSON data to MQTT
    client.publish(mqtt_topic, json);

    // Wait before sending new data
    delay(2000);  // Delay for 2 seconds between readings
}

//const obj=JSON.parse(msg.payload);
//var tempre = obj.Temp;
//msg.payload=tempre
//return msg;
