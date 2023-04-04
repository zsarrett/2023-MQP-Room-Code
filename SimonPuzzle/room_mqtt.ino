// /*
//     File Created: Apr. 3, 2023
//     https://randomnerdtutorials.com/esp32-mqtt-publish-subscribe-arduino-ide/
// */

// #include <WiFi.h>

// // Replace the next variables with your SSID/Password combination
// const char *ssid = "WPI-Open";
// const char *password = NULL;
// const char *ID = "website_mqtt_test";

// // Add your MQTT Broker IP address, example:
// // const char* mqtt_server = "192.168.1.144";
// const char *mqtt_server = "mqtt.eclipseprojects.io";

// WiFiClient espClient;
// PubSubClient client(espClient);

// /* Publishers -> "room/..."  */
// const char *room_state = "room/state";
// /* Subscribers -> "robot/..."  */
// const char *receive_code = "robot/test";
// const char *marker_id = "robot/aruco";

// long lastMsg = 0;
// char msg[50];
// int value = 0;

// void setup()
// {
//   Serial.begin(115200);

//   setup_wifi();
//   client.setServer(mqtt_server, 1883);
//   client.setCallback(callback);
// }

// void setup_wifi()
// {
//   delay(10);
//   // We start by connecting to a WiFi network
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     Serial.print("connecting...");
//     delay(500);
//   }

//   Serial.println("WiFi connected");
// }

// void callback(char *topic, byte *message, unsigned int length)
// {
//   Serial.print("Message arrived on topic: ");
//   Serial.print(topic);
//   Serial.print(". \tMessage: ");
//   String messageTemp;

//   // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
//   // Changes the output state according to the message
//   if (String(topic) == receive_code)
//   {
//     Serial.println("message received!");
//   }
// }

// void reconnect()
// {
//   // Loop until we're reconnected
//   while (!client.connected())
//   {
//     Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (client.connect(ID))
//     {
//       Serial.println("connected");
//       // Subscribe
//       client.subscribe(receive_code);
//       client.subscribe(marker_id);
//     }
//     else
//     {
//       WiFi.disconnect();
//       WiFi.reconnect();
//       Serial.println(" try again in 2 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
//   }
// }

// void check_connection()
// {
//   if (!client.connected())
//   {
//     reconnect();
//   }
//   client.loop();
// }

// void loop()
// {
//   check_connection();

//   long now = millis();
//   if (now - lastMsg > 5000)
//   {
//     client.publish(room_state, "testing room mqtt...");
//   }
// }