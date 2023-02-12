// Motor A connections
int pwmA = 5;
int in1 = #;
int in2 = #;

void setup() {
  // put your setup code here, to run once:
  pinMode(pwmA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);

  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

}

void loop() {

  analogWrite(pwmA, 255);
	// Turn on motor A & B
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);
	delay(2000);
  digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
  // put your main code here, to run repeatedly:

}
