/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
//#include "analogWrite.h"


const int yellowLED = 14;
const int redLED = 27;
const int blueLED = 33;
int brightnessY;  // The value read by the photoresistor
int brightnessR;
int brightnessB;
bool redOn;
bool blueOn;
bool yellowOn;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  redOn = false;
  blueOn = false;
  yellowOn = false;

  //Serial.println("Setup done");
}

bool simonRed() {
  brightnessR = analogRead(34);    // Read the brightness
  brightnessR = brightnessR / 16;  // Adjust the brightness value

  Serial.print("Red");
  Serial.println(brightnessR);

  if (brightnessR < 200 && redOn == false) {
   // Serial.println("Flashing RED");
    digitalWrite(redLED, LOW);  // turn off light
    delay(500);
    digitalWrite(redLED, HIGH);  // turn on the LED
    delay(500);                  // wait for half a second or 500 milliseconds
    digitalWrite(redLED, LOW);   // turn off the LED
    delay(500);                  // wait for half a second or 500 milliseconds
  } else {
    digitalWrite(redLED, HIGH);  // turn on light
    delay(10);                   // Wait 10 ms//digitalWrite (ledPin2, HIGH);
    //Serial.println("RED FOUND");
    redOn = true;
    return true;
  }
  return false;
}

bool simonBlue() {


  if (simonRed() == true && blueOn == false) {

    brightnessB = analogRead(32);
    brightnessB = brightnessB / 16;

    Serial.println("Brightness B: "  + brightnessB);

    if (brightnessB < 200) {
      //Serial.println("Flash BLUE");
      digitalWrite(blueLED, LOW);  // turn off light
      delay(500);
      digitalWrite(blueLED, HIGH);  // turn on the LED
      delay(500);                   // wait for half a second or 500milliseconds
      digitalWrite(blueLED, LOW);   // turn off the LED
      delay(500);                   // wait for half a second or 500milliseconds
    } else {
      digitalWrite(blueLED, HIGH);
      delay(10);
      //Serial.println("BLUE FOUND");
      blueOn = true;
      return true;
    }
  }
  return false;  
}

bool simonYellow() {
  if (simonBlue() == true && yellowOn == false) {

    brightnessY = analogRead(35);
    brightnessY = brightnessY / 16;

    Serial.print("Brightness Y: ");
    //Serial.println(brightnessY);


    if (brightnessY < 200) {
      //Serial.println("YELLOW FLASH");
      digitalWrite(yellowLED, LOW);  // turn off light
      delay(500);
      digitalWrite(yellowLED, HIGH);  // turn on the LED
      delay(500);                     // wait for half a second or 500 milliseconds
      digitalWrite(yellowLED, LOW);   // turn off the LED
      delay(500);                     // wait for half a second or 500 milliseconds
    } else {
      //Serial.println("FOUND YELLOW");
      digitalWrite(yellowLED, HIGH);
      delay(10);
      yellowOn = true;
      return true;
    }
  }
  return false;
}

bool simon() {
  //Serial.println("START RED");
  if (redOn == false){simonRed();}
  //Serial.println("END RED");
  //Serial.println("START BLUE");
  if (blueOn == false){simonBlue();}
  //Serial.println("END BLUE");
 // Serial.println("START YELLOW");
  if (yellowOn == false){simonYellow();}
  //Serial.println("END YELLOW");

  return true;
}

void loop() {
  //Serial.println("HIHI");

  simon();
  //analogWrite (yellowLED, brightness); // Put the value read for the LED
}
