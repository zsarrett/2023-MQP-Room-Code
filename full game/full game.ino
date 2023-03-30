#include <WiFi.h>
#include <PubSubClient.h>
//#include "analogWrite.h"

/* Simon Setup*/ 
const int yellowLED = 14;
const int redLED = 27;
const int blueLED = 33;
int brightnessY;  // The value read by the photoresistor
int brightnessR;
int brightnessB;
bool redOn;
bool blueOn;
bool yellowOn;
bool simonDone;

/*Door Setup*/
const int AIN1 = 36;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 37;            //control pin 2 on the motor driver for the right motor
const int PWMA = 26;  
int count;

/* MQTT Stuff */
const char *ssid = "WPI-Open";
const char *password = NULL;
const char *ID = "esp_boi";  // Name of our device, must be unique
IPAddress broker(130, 215, 120, 229); // IP address of Raspberry Pi ***WILL CHANGE***
WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT 


/* Publishers -> "esp32/..."  */
const char *room_state = "esp32/state";  
/* Subscribers -> "rpi/..."  */
const char *receive_code = "rpi/passcode";
const char *marker_id = "rpi/aruco";

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  redOn = false;
  blueOn = false;
  yellowOn = false;
  simonDone = false;

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


  if (redOn == true && blueOn == false) {

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
  if (blueOn == true && yellowOn == false) {

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
  simonDone = true;

  return true;
}

bool password(){
  if (simonDone == true){
    // accept password on website
  }
}

bool door(){
    count = 0;
    if (count == 0){  // & message of password recieved
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
    digitalWrite(PWMA, HIGH);               //now that the motor direction is set, drive it at max speed
    delay(2000);

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
    count = 1;
    }
    else {
      return true;
    }
}

//if blueOn == false and redOn == true

void loop() {
  //Serial.println("HIHI");

  simon();
  password();
  door();

  //analogWrite (yellowLED, brightness); // Put the value read for the LED
}



void pub(const char* topic, char* msg){
  client.publish(topic, msg);
}
void sub(const char* topic){
  client.subscribe(topic);
}

void reconnect() {
  char* recn = "connecting...";
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    display_lcd(recn);
    if (client.connect(ID)) {
      //Write ALL Subscribers
      sub(receive_code);
      sub(marker_id);
    } else {
      WiFi.disconnect();
      WiFi.reconnect();
      Serial.println(" try again in 2 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void check_connection(){
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

/* Handles ALL Subscribers */
void callback(char* topic, byte* message, unsigned int length) {
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

//  Serial.print(messageTemp);
  
  if (String(topic) == receive_code) {
    if(messageTemp == "accepted"){
      passcodeAccepted = true;
    }
    else if(messageTemp == "denied"){
      passcodeAccepted = false;
    }
  }

  if (String(topic) == marker_id) {
    if(messageTemp == "[1]"){
      nearCabinet = 1;
    }
    else if(messageTemp == "[2]"){
      nearCabinet = 2;
    }
    else if(messageTemp == "[3]"){
      nearCabinet = 3;
    }
  }
}
