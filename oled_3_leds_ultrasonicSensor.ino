#include <DHT.h>
#include <Adafruit_SSD1306.h>
#include <OakOLED.h>

// Ultrasonic Sensor Pins
const int trigger = D5;
const int echo = D6;

// LED Pins
const int led1 = D3;
const int led2 = D0;
const int led3 = D9;

// Distance Variables
long duration = 0;
float cm = 0;
float inch = 0;

// OLED Display
OakOLED oled;

void setup() {
  Serial.begin(9600);

  // Initialize OLED
  oled.begin();
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(1);
  oled.setCursor(0, 0);
  oled.println("Initializing...");
  oled.display();
  delay(2000);  

  // Set pin modes
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Turn off all LEDs initially
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void loop() {
  // Trigger the ultrasonic sensor
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  // Measure the echo duration
  duration = pulseIn(echo, HIGH);

  // Calculate distances
  cm = duration * 0.034 / 2;   // Convert to cm
  inch = duration * 0.0133 / 2; // Convert to inches

  // Print to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.print(" cm, ");
  Serial.print(inch);
  Serial.println(" inches");

  // Display on OLED
  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setCursor(0, 0);
  oled.println("Distance:");
  oled.setTextSize(2);
  oled.setCursor(0, 20);
  oled.print(cm);
  oled.println(" cm");
  oled.setCursor(0, 40);
  oled.print(inch);
  oled.println(" in");
  oled.display();

  // Control LEDs based on distance
  if (cm < 10) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  } else if (cm >= 10 && cm <= 20) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
  } else if (cm > 20) {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
  } else {
    // Turn off all LEDs if no valid reading
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
  }

  // Wait before next reading
  delay(1000);
}
