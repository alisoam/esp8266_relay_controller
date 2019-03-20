#include "mqtt.hpp"

#include "switches.hpp"
#include "parameters.hpp"
#include "peripherals.hpp"

PubSubClient mqttClient(espClient);



#define TOPIC_BUFFER_LEN    40
static char topicBuffer[TOPIC_BUFFER_LEN];

static void startMqtt()
{
  mqttClient.setServer(getMqttHostname(), getMqttPort());
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
  for (unsigned int index = 0; index < SWITCHES_NUMBER; index++)
  {
    snprintf(topicBuffer, TOPIC_BUFFER_LEN, "%s/sw%u", getDeviceName(), index + 1);
    if (strcmp(topic, topicBuffer) == 0)
    {
      if (length == 2 && strncmp((char*)payload, "ON", 2) == 0)
        switchSet(index, true);
      else if (length == 3 && strncmp((char*)payload, "OFF", 3) == 0)
        switchSet(index, false);
      char buffer[20];
      snprintf(buffer, 20, "SW%u is %s", index + 1, (switchGet(index)? "ON": "OFF"));
      Serial.println(buffer);
    }
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
    int ret = snprintf(buffer + 1 + start, bufferLen + 2 - start, "sw%u:\"%s\",", index + 1, (switchGet(index)? "ON": "OFF"));
    if (ret < 0)
      return;
    start += ret;
    if (bufferLen + 2 < start)
      start = bufferLen + 2;
  }
  buffer[start] = '}';
  buffer[start + 1] = '\0';
  snprintf(topicBuffer, TOPIC_BUFFER_LEN, "%s/status", getDeviceName());
  mqttClient.publish(topicBuffer, buffer);
  Serial.print("published: \"");
  Serial.print(buffer);
  Serial.println("\"");
}

void setupMqtt()
{
  mqttClient.setCallback(mqttCallback);
  startMqtt();
}

void restartMqtt()
{
  mqttClient.disconnect();
  startMqtt();
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
      if (((strlen(getMqttUsername()) > 0) && mqttClient.connect(getDeviceName(), getMqttUsername(), getMqttPassword())) || 
          ((strlen(getMqttUsername()) == 0) && mqttClient.connect(getDeviceName())))
      {
        Serial.println("mqtt connected");
        publishStatus();
      }
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
      snprintf(topicBuffer, TOPIC_BUFFER_LEN, "%s/#", getDeviceName());
      subscribed = mqttClient.subscribe(topicBuffer);
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
