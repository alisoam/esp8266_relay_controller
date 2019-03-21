#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdint>

void setWifiSsid(const char* str, bool restart = true);
void setWifiPassword(const char* str, bool restart = true);
void setWifiActive(bool active, bool restart = true);

const char* getWifiSsid();
const char* getWifiPassword();
bool getWifiActive();

void setApSsid(const char* str, bool restart = true);
void setApPassword(const char* str, bool restart = true);
void setApActive(bool active, bool restart = true);

const char* getApSsid();
const char* getApPassword();
bool getApActive();

void setMqttHostname(const char* str, bool restart = true);
void setMqttUsername(const char* str, bool restart = true);
void setMqttPassword(const char* str, bool restart = true);
void setMqttPort(uint16_t port, bool restart = true);
void setMqttActive(bool active, bool restart = true);

const char* getMqttHostname();
const char* getMqttUsername();
const char* getMqttPassword();
uint16_t getMqttPort();
bool getMqttActive();

void setDeviceName(const char* str, bool restart = true);
const char* getDeviceName();

void readParameters();
void setupParameters();
#endif
