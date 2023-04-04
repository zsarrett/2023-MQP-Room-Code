/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
// #include "analogWrite.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

const int yellowLED = 14;
const int redLED = 27;
const int blueLED = 33;
int brightnessY; // The value read by the photoresistor
int brightnessR;
int brightnessB;
bool redOn;
bool blueOn;
bool yellowOn;

int count;

const int AIN1 = 36;
const int AIN2 = 37;
const int PWMA = 26;

// --- MQTT Setup ---
// Replace the next variables with your SSID/Password combination
const char *ssid = "WPI-Open";
const char *password = NULL;
const char *ID = "website_mqtt_test";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "mqtt.eclipseprojects.io";

WiFiClient espClient;
PubSubClient client(espClient);

/* Publishers -> "room/..."  */
const char *room_state = "room/state";
/* Subscribers -> "robot/..."  */
const char *receive_code = "robot/test";
const char *marker_id = "robot/aruco";

long lastMsg = 0;
char msg[50];
int value = 0;

void mqtt_setup()
{
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

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
  count = 0;

  // Serial.println("Setup done");
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("connecting...");
    delay(500);
  }

  Serial.println("WiFi connected");
}

void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". \tMessage: ");
  String messageTemp;

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == receive_code)
  {
    Serial.println("message received!");
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe(receive_code);
      client.subscribe(marker_id);
    }
    else
    {
      WiFi.disconnect();
      WiFi.reconnect();
      Serial.println(" try again in 2 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void check_connection()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

// --- MQTT Setup Ends ---

bool simonRed()
{
  brightnessR = analogRead(34);   // Read the brightness
  brightnessR = brightnessR / 16; // Adjust the brightness value

  Serial.print("Red");
  Serial.println(brightnessR);

  if (brightnessR < 200 && redOn == false)
  {
    // Serial.println("Flashing RED");
    digitalWrite(redLED, LOW); // turn off light
    delay(500);
    digitalWrite(redLED, HIGH); // turn on the LED
    delay(500);                 // wait for half a second or 500 milliseconds
    digitalWrite(redLED, LOW);  // turn off the LED
    delay(500);                 // wait for half a second or 500 milliseconds
  }
  else
  {
    digitalWrite(redLED, HIGH); // turn on light
    delay(10);                  // Wait 10 ms//digitalWrite (ledPin2, HIGH);
    // Serial.println("RED FOUND");
    redOn = true;
    return true;
  }
  return false;
}

bool simonBlue()
{
  if (redOn == true && blueOn == false)
  {

    brightnessB = analogRead(32);
    brightnessB = brightnessB / 16;

    Serial.println("Brightness B: " + brightnessB);

    if (brightnessB < 200)
    {
      // Serial.println("Flash BLUE");
      digitalWrite(blueLED, LOW); // turn off light
      delay(500);
      digitalWrite(blueLED, HIGH); // turn on the LED
      delay(500);                  // wait for half a second or 500milliseconds
      digitalWrite(blueLED, LOW);  // turn off the LED
      delay(500);                  // wait for half a second or 500milliseconds
    }
    else
    {
      digitalWrite(blueLED, HIGH);
      delay(10);
      // Serial.println("BLUE FOUND");
      blueOn = true;
      return true;
    }
  }
  return false;
}

bool simonYellow()
{
  if (blueOn == true && yellowOn == false)
  {

    brightnessY = analogRead(35);
    brightnessY = brightnessY / 16;

    Serial.print("Brightness Y: ");
    // Serial.println(brightnessY);

    if (brightnessY < 200)
    {
      // Serial.println("YELLOW FLASH");
      digitalWrite(yellowLED, LOW); // turn off light
      delay(500);
      digitalWrite(yellowLED, HIGH); // turn on the LED
      delay(500);                    // wait for half a second or 500 milliseconds
      digitalWrite(yellowLED, LOW);  // turn off the LED
      delay(500);                    // wait for half a second or 500 milliseconds
    }
    else
    {
      // Serial.println("FOUND YELLOW");
      digitalWrite(yellowLED, HIGH);
      delay(10);
      yellowOn = true;
      return true;
    }
  }
  return false;
}

bool simon()
{
  // Serial.println("START RED");
  if (redOn == false)
  {
    simonRed();
  }
  // Serial.println("END RED");
  // Serial.println("START BLUE");
  if (blueOn == false)
  {
    simonBlue();
  }
  // Serial.println("END BLUE");
  // Serial.println("START YELLOW");
  if (yellowOn == false)
  {
    simonYellow();
  }
  // Serial.println("END YELLOW");

  // door();

  // Todo SEND MESSAGE SEND HERE!!!!!!!!

  return true;
}

void door()
{
  if (yellowOn == true && count == 0)
  {                           // WAIT FOR RESPONSE FROM WEBSITE ALSO IN THIS STATEMENT
                              // drive motor forward (positive speed)
    digitalWrite(AIN1, HIGH); // set pin 1 to high
    digitalWrite(AIN2, LOW);  // set pin 2 to low
    digitalWrite(PWMA, HIGH); // now that the motor direction is set, drive it at max speed
    delay(1800);

    // //drive motor backward (negative speed)
    // digitalWrite(AIN1, LOW);                          //set pin 1 to low
    // digitalWrite(AIN2, HIGH);                         //set pin 2 to high
    // digitalWrite(PWMA, HIGH);               //now that the motor direction is set, drive it at max speed
    // delay(3000);

    // stop motor
    digitalWrite(AIN1, LOW); // set pin 1 to low
    digitalWrite(AIN2, LOW); // set pin 2 to low
    digitalWrite(PWMA, LOW); // now that the motor direction is set, stop motor
    delay(3000);
    count = count + 1;
  }
}

// if blueOn == false and redOn == true`x
void loop()
{
  // Serial.println("HIHI");

  check_connection();
  long now = millis();
  if (now - lastMsg > 5000)
  {
    client.publish(room_state, "testing room mqtt...");
  }

  simon();
  delay(100);
  door();

  // analogWrite (yellowLED, brightness); // Put the value read for the LED
}
