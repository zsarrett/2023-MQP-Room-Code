// Motor A connections
const int AIN1 = 36;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 37;            //control pin 2 on the motor driver for the right motor
const int PWMA = 26;            //speed control pin on the motor driver for the right motor

//VARIABLES
int motorSpeed = 0;       //starting speed for the motor

void setup() {
  //set the motor contro pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
}

void loop() {
    //drive motor forward (positive speed)
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    digitalWrite(PWMA, HIGH);               //now that the motor direction is set, drive it at max speed
    delay(3000);

    // //drive motor backward (negative speed)
    // digitalWrite(AIN1, LOW);                          //set pin 1 to low
    // digitalWrite(AIN2, HIGH);                         //set pin 2 to high
    // digitalWrite(PWMA, HIGH);               //now that the motor direction is set, drive it at max speed
    // delay(3000);

    //stop motor
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    digitalWrite(PWMA, LOW);               //now that the motor direction is set, stop motor
    delay(3000);
}
