#ifndef MQTT_HPP
#define MQTT_HPP

#include <PubSubClient.h>

extern PubSubClient mqttClient;

void setupMqtt();
void restartMqtt();
void mqttConnectionCheck();
void publishStatus();

#endif 
