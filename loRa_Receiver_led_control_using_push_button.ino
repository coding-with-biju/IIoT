#include <SPI.h>
#include <LoRa.h>

#define ss 10       // LoRa module pin definitions
#define rst 9
#define dio0 2
#define ledPin 7   // Optional LED for feedback

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Receiver");

  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(866E6)) { // 866 MHz for Europe
    Serial.print(".");
    delay(500);
  }

  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  pinMode(ledPin, OUTPUT); // Optional LED for feedback
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    digitalWrite(ledPin, HIGH);

    Serial.print("Received packet: ");
    while (LoRa.available()) {
      char c = LoRa.read();
      Serial.print(c);
    }
    Serial.println();

    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());

    delay(500); 
    digitalWrite(ledPin, LOW);
  }
}
