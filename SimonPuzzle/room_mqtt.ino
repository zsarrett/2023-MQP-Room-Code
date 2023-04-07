/*
    Go to thingspeak.com and create an account if you don't have one already.
    After logging in, click on the "New Channel" button to create a new channel for your data. This is where your data will be stored and displayed.
    Fill in the Name, Description, and other fields for your channel as desired, then click the "Save Channel" button.
    Take note of the "Write API Key" located in the "API keys" tab, this is the key you will use to send data to your channel.
    Replace the channelID from tab "Channel Settings" and privateKey with "Read API Keys" from "API Keys" tab.
    Replace the host variable with the thingspeak server hostname "api.thingspeak.com"
    Upload the sketch to your ESP32 board and make sure that the board is connected to the internet. The ESP32 should now send data to your Thingspeak channel at the intervals specified by the loop function.
    Go to the channel view page on thingspeak and check the "Field1" for the new incoming data.
    You can use the data visualization and analysis tools provided by Thingspeak to display and process your data in various ways.
    Please note, that Thingspeak accepts only integer values.

    You can later check the values at https://thingspeak.com/channels/2005329
    Please note that this public channel can be accessed by anyone and it is possible that more people will write their values.
 */

#include <WiFi.h>
#include <PubSubClient.h>

// const char* host = "api.thingspeak.com"; // This should not be changed
const int httpPort = 80; // This should not be changed
// const String channelID   = "2005329"; // Change this to your channel ID
// const String writeApiKey = "V6YOTILH9I7D51F9"; // Change this to your Write API key
// const String readApiKey = "34W6LGLIFXD56MPM"; // Change this to your Read API key
/* MQTT Stuff */

const char *ssid = "WPI-Open";
const char *password = NULL;
const char *ID = "esp_boi";  // Name of our device, must be unique
const char *mqtt_server = "mqtt.eclipseprojects.io";
int counter = 0;
WiFiClient wclient;
PubSubClient client(wclient);  // Setup MQTT


/* Publishers -> "esp32/..."  */
const char *room_state = "esp32/state";
/* Subscribers -> "rpi/..."  */
const char *receive_code = "rpi/passcode";
const char *marker_id = "rpi/aruco";


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(100); }

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println("******************************************************");
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  counter++; 
  check_connection();
  sub("robot/test");
  pub("room/test", "hi");
}

/************************ MQTT Methods ************************/
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

  Serial.println(messageTemp);

}
/*************************************************************/