/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <string> // std::string, std::stoi
#include <WiFi.h>
#define MQTT_VERSION MQTT_VERSION_3_1
#include <PubSubClient.h>
#include <Wire.h>

using namespace std;

void setup_wifi();
void callback(char *topic, byte *message, unsigned int length);

// Replace the next variables with your SSID/Password combination
const char *ssid = "router-wifi-2098";
const char *password = "govtech123";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.108.6";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
volatile uint64_t lastTime;
volatile uint8_t toSend = 112;
volatile uint8_t lastSent = 112;

// LED Pin
const int ledPin = 4;

void setup()
{
  Serial.begin(9600, SERIAL_8N1);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  // status = bme.begin();

  setup_wifi();
  client.setServer(mqtt_server, 8000);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);
  lastTime = millis();
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  // Serial.println();
  // Serial.print("Connecting to ");
  // Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // Serial.print(".");
  }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  // cli();

  // if (message[0] != lastSent)
  // {
  //   lastSent = message[0];
  Serial.write(message[0]);
  // Serial.println(message[0]);
  // }
  // sei();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    // Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client"))
    {
      // Serial.println("connected");
      // Subscribe
      client.subscribe("joystick/value");
    }
    else
    {
      // Serial.print("failed, rc=");
      // Serial.print(client.state());
      // Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }

  client.loop();
}