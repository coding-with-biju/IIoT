#include <Servo.h>

const int trigPin = 9;  // Ultrasonic sensor TRIG pin
const int echoPin = 10; // Ultrasonic sensor ECHO pin
const int servoPin = 11; // Servo signal pin
const int servoMin = 0;  // Minimum servo angle
const int servoMax = 90; // Maximum servo angle

Servo myServo; // Create servo object

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
}
void loop() {
  long duration, distance;

  // Trigger ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo pin
  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034 / 2); // Calculate distance in cm

  // Control servo based on distance
  if (distance < 20) {
    myServo.write(servoMin); // Move servo to minimum angle
  } else if (distance > 50) {
    myServo.write(servoMax); // Move servo to maximum angle
  } else {
    myServo.write(map(distance, 20, 50, servoMin, servoMax)); // Move servo proportionally
  }

  delay(100);
}

