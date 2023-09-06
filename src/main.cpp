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

#define MOTORSPEED7 255
#define MOTORSPEED6 220
#define MOTORSPEED5 195
#define MOTORSPEED4 175
#define MOTORSPEED3 150
#define MOTORSPEED2 135
#define MOTORSPEED1 120
#define MOTORSPEED0 0

#define LEFTFORWARD 27
#define LEFTBACKWARD 14
#define RIGHTFORWARD 12
#define RIGHTBACKWARD 13

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
  setCpuFrequencyMhz(240);

  analogWrite(RIGHTBACKWARD, LOW);
  analogWrite(RIGHTFORWARD, LOW);
  analogWrite(LEFTBACKWARD, LOW);
  analogWrite(LEFTFORWARD, LOW);

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

int getSpeedFromRep(int representation)
{
  int speed;
  switch (representation)
  {
  case 0:
    speed = -MOTORSPEED7;
    break;
  case 1:
    speed = MOTORSPEED6;
    break;
  case 2:
    speed = MOTORSPEED5;
    break;
  case 3:
    speed = MOTORSPEED4;
    break;
  case 4:
    speed = MOTORSPEED3;
    break;
  case 5:
    speed = MOTORSPEED2;
    break;
  case 6:
    speed = MOTORSPEED1;
    break;
  case 7:
    speed = MOTORSPEED0;
    break;
  case 8:
    speed = MOTORSPEED1;
    break;
  case 9:
    speed = MOTORSPEED2;
    break;
  case 10:
    speed = MOTORSPEED3;
    break;
  case 11:
    speed = MOTORSPEED4;
    break;
  case 12:
    speed = MOTORSPEED5;
    break;
  case 13:
    speed = MOTORSPEED6;
    break;
  case 14:
    speed = MOTORSPEED7;
    break;
  default:
    speed = 0;
    break;
  }

  return speed;
}

void updateLeftMotor(int representation)
{

  int speed = getSpeedFromRep(representation);
  if (representation < 7)
  {
    analogWrite(LEFTFORWARD, LOW);
    analogWrite(LEFTBACKWARD, speed);
  }
  else
  {
    analogWrite(LEFTFORWARD, speed);
    analogWrite(LEFTBACKWARD, LOW);
  }
}

void updateRightMotor(int representation)
{
  int speed = getSpeedFromRep(representation);
  if (representation < 7)
  {
    analogWrite(RIGHTFORWARD, LOW);
    analogWrite(RIGHTBACKWARD, speed);
  }
  else
  {
    analogWrite(RIGHTFORWARD, speed);
    analogWrite(RIGHTBACKWARD, LOW);
  }
}

void sendMotor(int bignum)
{
  int motor1 = bignum / 15;
  int motor2 = bignum % 15;
  updateLeftMotor(motor1);
  updateRightMotor(motor2);
}

void callback(char *topic, byte *message, unsigned int length)
{
  string messageTemp;
  for (int i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }
  toSend = stoi(messageTemp);
  Serial.println(toSend);
  sendMotor(toSend);
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

  client.loop();
}