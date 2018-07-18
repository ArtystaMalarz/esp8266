#include <ESP8266WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>

#include "DHT.h"

#define DHTPIN 5 // what digital pin we're connected to NodeMCU (D6)

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11

//#define DHTTYPE DHT22 // DHT 22 (AM2302), AM2321
//#define DHTTYPE DHT21 // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors. This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.





DHT dht(DHTPIN, DHTTYPE);

#define wifi_ssid "nazwa sieci"
#define wifi_password "haslo"

#define mqtt_server "adres serwera mqtt"

#define mqtt_user "login"
#define mqtt_password "haslo"

//topics:

#define swiatlo "ArtystaMalarz/feeds/light"
#define led "ArtystaMalarz/feeds/led"



WiFiClient espClient;
PubSubClient client(espClient);



////////////////////////odczyt stanu przelacdznika


 
void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 
 if (strcmp(topic,"ArtystaMalarz/feeds/light")== 0)
 {
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0'){
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(0, HIGH);
   Serial.print("przekaznik1 wlaczony!");
  }
  if (receivedChar == '1')
   digitalWrite(0, LOW);
  }
  Serial.println();
 }

//////////////////////pezkaznik2
 if (strcmp(topic,"ArtystaMalarz/feeds/led")== 0)
 {
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0'){
  // ESP8266 Huzzah outputs are "reversed"
  digitalWrite(4, HIGH);
   Serial.print("przekaznik2 wlaczony!");
  }
  if (receivedChar == '1')
   digitalWrite(4, LOW);
  }
  Serial.println();
 }


 
}
 
 






/////////////////////////



void setup() {

  pinMode(4, OUTPUT);
   pinMode(0, OUTPUT);
     digitalWrite(4, HIGH);
       digitalWrite(0, HIGH);
  
  Serial.begin(9600);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//////
   client.setCallback(callback);
 
 //////////
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}




void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
    
          // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;  
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      clientName += "-";
      clientName += String(micros() & 0xff, 16);
      Serial.print("Connecting to ");
      Serial.print(mqtt_server);
      Serial.print(" as ");
      Serial.println(clientName);
      


    // Attempt to connect
    // If you do not want to use a username and password, change next line to
 //if (client.connect((char*) clientName.c_str())) {
       if (client.connect((char*) clientName.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("connected");
        client.subscribe("ArtystaMalarz/feeds/light");
  client.subscribe("ArtystaMalarz/feeds/led");
 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void loop() {
  
      if (!client.connected()) {
        reconnect();
      }
      client.loop();

    
}

