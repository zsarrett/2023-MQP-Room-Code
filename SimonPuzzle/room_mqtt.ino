/*
    File Created: Apr. 3, 2023
    File Description: this file aims to test whether wifi and mqtt works on esp32. 
*/

#include <WiFi.h>
#include <PubSubClient.h>

const int httpPort = 80;
const char *ssid = "WPI-Open";
const char *password = NULL;
const char *ID = "room_mqtt";
const char *mqtt_server = "mqtt.eclipseprojects.io";

WiFiClient wclient;
PubSubClient client(wclient); 


void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(100); }

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