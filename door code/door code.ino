#include "Arduino.h"
#include "analogWrite.h"
#include "WiFi.h"
// Motor A connections
int pwmA = 19;
int in1 = 18;
int in2 = 23;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pwmA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);

  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

}

void loop() {

  analogWrite(pwmA, 250);
	// Turn on motor A & B
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);
	delay(2000);
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  // put your main code here, to run repeatedly:

}
