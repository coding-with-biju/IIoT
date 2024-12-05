#include <SPI.h>
#include <LoRa.h>

#define ss 10
#define rst 9
#define dio0 2
#define buttonPin 7
// #define ledPin 13

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Transmitter");
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(866E6)) {
  Serial.println(".");
  delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin, OUTPUT);
}

void loop(){
  if (digitalRead(buttonPin) == LOW) {  // Button is pressed (active LOW)
    Serial.println("Button pressed! Sending message...");
    // digitalWrite(ledPin, HIGH);
    LoRa.beginPacket();
    LoRa.print("LoRa!");
    LoRa.endPacket();
    delay(500); // LED stays on for half a second
    // digitalWrite(ledPin, LOW);
    delay(300); // Adjust if necessary
}
}

