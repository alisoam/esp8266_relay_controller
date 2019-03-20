#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdint>

void setWifiSsid(const char* str);
void setWifiPassword(const char* str);

const char* getWifiSsid();
const char* getWifiPassword();

void setApSsid(const char* str);
void setApPassword(const char* str);

const char* getApSsid();
const char* getApPassword();

void setMqttHostname(const char* str);
void setMqttUsername(const char* str);
void setMqttPassword(const char* str);
void setMqttPort(uint16_t port);

const char* getMqttHostname();
const char* getMqttUsername();
const char* getMqttPassword();
uint16_t getMqttPort();

void setDeviceName(const char* str);
const char* getDeviceName();

void readParameters();
void setupParameters();
#endif
