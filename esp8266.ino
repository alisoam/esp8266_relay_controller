#include <cstdio>

#include <Wire.h>
#include <RTClib.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "eeprom.h"

#define WIFI_SSID "Home 2"
#define WIFI_PASSWORD "74221776"

//#define WIFI_SSID "AndroidAP"
//#define WIFI_PASSWORD "swdm6003"

#define MQTT_SERVER   "m16.cloudmqtt.com"
#define MQTT_PORT     17556 
#define MQTT_USER     "fegauiij"
#define MQTT_PASSWORD "ttDevdrkiuct"

#define DEVICE_NAME   "switch1"

#define SWITCHES_NUMBER 4

void mqttCallback(const char* topic, byte* payload, unsigned int length);

WiFiClient espClient;
PubSubClient mqttClient(MQTT_SERVER, MQTT_PORT, mqttCallback, espClient);
RTC_DS1307 rtc;

bool switches[SWITCHES_NUMBER] = {false};
const uint8_t switchOut[SWITCHES_NUMBER] = {15, 10, 12, 16};
const uint8_t switchFeedbackIn[SWITCHES_NUMBER] = {0, 2, 13, 14};

void setupRtc()
{
  delay(10);
  if (! rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void setupWifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMqtt()
{
}

void setupSwitch()
{
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    digitalWrite(switchOut[index], LOW);
    pinMode(switchOut[index], OUTPUT);
  }
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    pinMode(switchFeedbackIn[index], INPUT);
  }
}
void mqttConnectionCheck()
{
  static bool subscribed = false;
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("wifi is connected");
    if (!mqttClient.connected())
    {
      subscribed = false;
      Serial.print("Attempting MQTT connection...");
      // If you do not want to use a username and password, change next line to
      // if (client.connect("ESP8266Client")) {
      if (mqttClient.connect("ESP8266Client", MQTT_USER, MQTT_PASSWORD))
        Serial.println("mqtt connected");
      else
      {
        Serial.print("mqtt connection failed, rc=");
        Serial.println(mqttClient.state());
      }
    }
    else
      Serial.println("mqtt is connected");
    if (mqttClient.connected() and not subscribed)
    {
      Serial.println("mqtt is subscribing to corresponding topic");
      subscribed = mqttClient.subscribe(DEVICE_NAME"/#");
    }
    else
      Serial.println("mqtt client is subscribed to corresponding topic");
  }
  else
  {
    Serial.println("wifi is not connected");
    subscribed = false;
  }
}

void switchSet()
{
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
    digitalWrite(switchOut[index], switches[index]);
}

void switchStatusCheck()
{
  switchSet();
}

void publishStatus()
{
  const unsigned int bufferLen = 50;
  char buffer[bufferLen + 3];
  buffer[0] = '{';
  unsigned int start = 0;
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    int ret = snprintf(buffer + 1 + start, bufferLen + 2 - start, "sw%u:\"%s\",", index + 1, (switches[index]? "ON": "OFF"));
    if (ret < 0)
      return;
    start += ret;
    if (bufferLen + 2 < start)
      start = bufferLen + 2;
  }
  buffer[start] = '}';
  buffer[start + 1] = '\0';
  mqttClient.publish(DEVICE_NAME"/status", buffer);
  Serial.print("published: \"");
  Serial.print(buffer);
  Serial.println("\"");
}

void mqttCallback(const char* topic, byte* payload, unsigned int length)
{
  Serial.println("MQTT Message");
  Serial.print("\tTopic: \"");
  Serial.print(topic);
  Serial.println("\"");
  Serial.print("\tMessage: \"");
  for (unsigned int i = 0; i < length; i++)
    Serial.print(*(char*)(payload + i));
  Serial.println("\"");
  bool change = false;
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    char topicCompairBuffer[20];
    snprintf(topicCompairBuffer, 20, "%s/sw%u", DEVICE_NAME, index + 1);
    if (strcmp(topic, topicCompairBuffer) == 0)
    {
      if (length == 2 && strncmp((char*)payload, "ON", 2) == 0)
        switches[index] = true;
      else if (length == 3 && strncmp((char*)payload, "OFF", 3) == 0)
        switches[index] = false;
        
      char buffer[20];
      snprintf(buffer, 20, "SW%u is %s", index + 1, (switches[index]? "ON": "OFF"));
      Serial.println(buffer);
      
      change = true;
    }
  }
  if (change)
  {
    switchSet();
    publishStatus();
  }
}

void setup()
{
  Serial.println("start");
  Serial.begin(115200);
  Wire.begin();
  setupSwitch();
  setupRtc();
  setupWifi();
  setupMqtt();
  Serial.println("initialization finished. starting main loop");
}

void loop()
{
  mqttClient.loop();

  static long lastCheck = 0;
  long now = millis();
  if (now < lastCheck or now - lastCheck > 2000)
  {
    lastCheck = now;

    mqttConnectionCheck();
    
    static int loopCounter = 0;
    char buffer[20];
    snprintf(buffer, 20, "loop %d", loopCounter++);
    Serial.println(buffer);  
  }
}
