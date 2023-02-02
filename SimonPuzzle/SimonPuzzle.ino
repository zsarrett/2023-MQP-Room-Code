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
int brightnessY; // The value read by the photoresistor
int brightnessR;
int brightnessB;

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(yellowLED, OUTPUT);
    pinMode(redLED, OUTPUT);
    pinMode(blueLED, OUTPUT);

    Serial.println("Setup done");
}

bool simonRed(){
   brightnessR= analogRead (34); // Read the brightness  
   brightnessR = brightnessR / 16; // Adjust the brightness value

   //Serial.println(brightness);

   if(brightnessR >  200)
    {
        digitalWrite (redLED, HIGH); // turn on light
        delay (10); // Wait 10 ms//digitalWrite (ledPin2, HIGH);
        Serial.print("RED FOUND");
        return true;
    }
    else {
      Serial.print("Flashing RED");
      digitalWrite (redLED, LOW); // turn off light
      delay(500);
      digitalWrite (redLED, HIGH);	// turn on the LED
      delay(500);	// wait for half a second or 500 milliseconds
      digitalWrite (redLED, LOW);	// turn off the LED
      delay(500);	// wait for half a second or 500 milliseconds
    }
}

 bool simonBlue(){
   if(simonRed() == true){
      brightnessB = analogRead(35);
      brightnessB = brightnessB / 16;
      if(brightnessB > 200){
       digitalWrite(blueLED, HIGH);
        delay(10);
        Serial.print("BLUE FOUND");
        return true;
   }
    else{
      Serial.print("Flash BLUE");
      digitalWrite (blueLED, LOW); // turn off light
      delay(500);
      digitalWrite (blueLED, HIGH);	// turn on the LED
      delay(500);	// wait for half a second or 500 milliseconds
      digitalWrite (blueLED, LOW);	// turn off the LED
      delay(500);	// wait for half a second or 500 milliseconds
   }
 }
 }

 bool simonYellow(){
   if(simonRed() == true && simonBlue() == true){
      brightnessY = analogRead(32);
      brightnessY = brightnessY / 16;
      if(brightnessY > 200){
        Serial.print("FOUND YELLOW");
       digitalWrite(yellowLED, HIGH);
        delay(10);
        return true;
   }
    else{
      Serial.print("YELLOW FLASH");
      digitalWrite (yellowLED, LOW); // turn off light
      delay(500);
      digitalWrite (yellowLED, HIGH);	// turn on the LED
      delay(500);	// wait for half a second or 500 milliseconds
      digitalWrite (yellowLED, LOW);	// turn off the LED
      delay(500);	// wait for half a second or 500 milliseconds
   }
   }

 }

 bool simon(){
   Serial.print("START RED");
   simonRed();
   Serial.print("END RED");
   Serial.print("START BLUE");
   simonBlue();
   Serial.print("END BLUE");
   Serial.print("START YELLOW");
   simonYellow();
   Serial.print("END YELLOW")

   return true;
 }

void loop()
{
    //Serial.println("HIHI");

   simon();
   //analogWrite (yellowLED, brightness); // Put the value read for the LED
   
}
