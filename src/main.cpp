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
const char *ssid = "Pereiras_2nd_floor";
const char *password = "volks12345";

// Add your MQTT Broker IP address, example:
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "192.168.2.37";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
volatile uint64_t lastTime;
volatile int toSend = 112;
int prevSend = 0;

// LED Pin
const int ledPin = 4;

void setup()
{
  Serial.begin(115200);
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
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *message, unsigned int length)
{
  string messageTemp;
  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  toSend = stoi(messageTemp);
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("joystick/value");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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

  // if (millis() - lastTime > 50)
  // {
  if (toSend != prevSend)
  {
    Serial.println(toSend);
    prevSend = toSend;
  }
  //   lastTime = millis();
  // }

  client.loop();
}