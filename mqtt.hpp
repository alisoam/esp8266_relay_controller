#ifndef MQTT_HPP
#define MQTT_HPP

extern PubSubClient mqttClient;

void setupMqtt();
void mqttConnectionCheck();
void publishStatus();

#endif
