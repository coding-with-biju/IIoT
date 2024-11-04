
int trigger = D1;
int echo = D2;
int led = D4;

long duration = 0;
int cm = 0;
int inch = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigger, LOW);
  digitalWrite(trigger, HIGH);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  cm = duration*0.034/2;
  inch = duration*0.0133/2;

  if (inch < 50){
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }

  if (inch < 100 ) {
    Serial.print("Inches: ");
    Serial.println(inch);
    Serial.print("Cm: ");
    Serial.println(cm);
  }
  delay(500);
}
