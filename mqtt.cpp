#include <PubSubClient.h>

#define MQTT_SERVER   "m16.cloudmqtt.com"
#define MQTT_PORT     17556 
#define MQTT_USER     "fegauiij"
#define MQTT_PASSWORD "ttDevdrkiuct"

#define DEVICE_NAME   "switch1"

void mqttCallback(const char* topic, byte* payload, unsigned int length);

PubSubClient mqttClient(MQTT_SERVER, MQTT_PORT, mqttCallback, espClient);

static void mqttCallback(const char* topic, byte* payload, unsigned int length)
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


void setupMqtt()
{
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
