#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <OakOLED.h> // OLED library

#define DHTPIN D4        
#define DHTTYPE DHT11    

OakOLED oled;            
DHT dht(DHTPIN, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  dht.begin();
  oled.begin();            
  oled.clearDisplay();     
  oled.setTextSize(4);     
  oled.setTextColor(1);    
  oled.setCursor(0, 0);    
}

void loop() {
  
  float temp = dht.readTemperature();  // Read temperature in Celsius
  float hum = dht.readHumidity();      // Read humidity percentage

  
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C ");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  oled.clearDisplay();
  
  oled.setCursor(0, 0);       
  oled.println("Temp: " + String(temp) + " C");
  oled.println("Hum: " + String(hum) + " %");
  oled.display();            
  
  delay(2000);  
}
