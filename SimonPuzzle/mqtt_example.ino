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
//  while(!Serial) {} //comment out unless testing!
//  /* setup MQTT protocols */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // Connect to network
  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
  }
  Serial.println("WiFi connected");
  client.setServer(broker, 1883);
  client.setCallback(callback); 
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
/*************************************************************/