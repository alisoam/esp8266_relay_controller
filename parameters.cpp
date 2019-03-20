#include "parameters.hpp"

#include <cstring>

#include "mqtt.hpp"
#include "eeprom.hpp"
#include "peripherals.hpp"

# ifndef EEPROM_ADDR
#   define EEPROM_ADDR (0x50)
# endif

#define EEPROM_DEVICE_NAME_LEN    30
#define EEPROM_WIFI_SSID_LEN      30
#define EEPROM_WIFI_PASSWORD_LEN  30
#define EEPROM_AP_SSID_LEN        30
#define EEPROM_AP_PASSWORD_LEN    30
#define EEPROM_MQTT_HOSTNAME_LEN  30
#define EEPROM_MQTT_USERNAME_LEN  30
#define EEPROM_MQTT_PASSWORD_LEN  30

#define EEPROM_START_ADDRESS  100
#define EEPROM_UNSIGNED_INT_START_ADDRESS   1024
#define EEPROM_BOOL_START_ADDRESS   1536

#define EEPROM_DEVICE_NAME_ADDRESS    (EEPROM_START_ADDRESS)
#define EEPROM_WIFI_SSID_ADDRESS      (EEPROM_DEVICE_NAME_ADDRESS    + EEPROM_DEVICE_NAME_LEN)
#define EEPROM_WIFI_PASSWORD_ADDRESS  (EEPROM_WIFI_SSID_ADDRESS      + EEPROM_WIFI_SSID_LEN)
#define EEPROM_AP_SSID_ADDRESS        (EEPROM_WIFI_PASSWORD_ADDRESS  + EEPROM_WIFI_PASSWORD_LEN)
#define EEPROM_AP_PASSWORD_ADDRESS    (EEPROM_AP_SSID_ADDRESS        + EEPROM_AP_SSID_LEN)
#define EEPROM_MQTT_HOSTNAME_ADDRESS  (EEPROM_AP_PASSWORD_ADDRESS    + EEPROM_AP_PASSWORD_LEN)
#define EEPROM_MQTT_USERNAME_ADDRESS  (EEPROM_MQTT_HOSTNAME_ADDRESS  + EEPROM_MQTT_HOSTNAME_LEN)
#define EEPROM_MQTT_PASSWORD_ADDRESS  (EEPROM_MQTT_USERNAME_ADDRESS  + EEPROM_MQTT_USERNAME_LEN)

#define EEPROM_MQTT_PORT_INDEX              100
#define EEPROM_UNSIGNED_INT_ADDRESS(index)  (EEPROM_UNSIGNED_INT_START_ADDRESS + index * EEPROM_UNSINGED_INT_SIZE)

#define EEPROM_WIFI_ACTUVE_INDEX              100
#define EEPROM_AP_ACTUVE_INDEX                101
#define EEPROM_MQTT_ACTUVE_INDEX              102
#define EEPROM_MQTT_SECURE_INDEX              103
#define EEPROM_BOOL_ADDRESS(index)  (EEPROM_BOOL_START_ADDRESS + index * EEPROM_BOOL_SIZE)
//
static char deviceName[EEPROM_DEVICE_NAME_LEN + 1];

static bool wifiActive;
static char wifiSsid[EEPROM_WIFI_SSID_LEN + 1];
static char wifiPassword[EEPROM_WIFI_PASSWORD_LEN + 1];

static bool apActive;
static char apSsid[EEPROM_AP_SSID_LEN + 1];
static char apPassword[EEPROM_AP_PASSWORD_LEN + 1];

static bool mqttActive;
static char mqttHostname[EEPROM_MQTT_HOSTNAME_LEN + 1];
static char mqttUsername[EEPROM_MQTT_USERNAME_LEN + 1];
static char mqttPassword[EEPROM_MQTT_PASSWORD_LEN + 1];

unsigned int mqttPort = 0;

void setWifiSsid(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_WIFI_SSID_ADDRESS, EEPROM_WIFI_SSID_LEN, str);
  strncpy(wifiSsid, str, EEPROM_WIFI_SSID_LEN);
  wifiSsid[EEPROM_WIFI_SSID_LEN] = 0;
  restartWifi();
}

const char* getWifiSsid()
{
  return wifiSsid;
}

void setWifiPassword(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_WIFI_PASSWORD_ADDRESS, EEPROM_WIFI_PASSWORD_LEN, str);
  strncpy(wifiPassword, str, EEPROM_WIFI_PASSWORD_LEN);
  wifiPassword[EEPROM_WIFI_PASSWORD_LEN] = 0;
  restartWifi();
}

const char* getWifiPassword()
{
  return wifiPassword;
}

void setApSsid(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_AP_SSID_ADDRESS, EEPROM_AP_SSID_LEN, str);
  strncpy(apSsid, str, EEPROM_AP_SSID_LEN);
  apSsid[EEPROM_AP_SSID_LEN] = 0;
  restartAp();
}

const char* getApSsid()
{
  return apSsid;
}

void setApPassword(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_AP_PASSWORD_ADDRESS, EEPROM_AP_PASSWORD_LEN, str);
  strncpy(apPassword, str, EEPROM_AP_PASSWORD_LEN);
  apPassword[EEPROM_AP_PASSWORD_LEN] = 0;
  restartAp();
}

const char* getApPassword()
{
  return apPassword;
}

void setMqttHostname(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_HOSTNAME_ADDRESS, EEPROM_MQTT_HOSTNAME_LEN, str);
  strncpy(mqttHostname, str, EEPROM_MQTT_HOSTNAME_LEN);
  mqttHostname[EEPROM_MQTT_HOSTNAME_LEN] = 0;
  restartMqtt();
}

const char* getMqttHostname()
{
  return mqttHostname;
}


void setMqttPort(uint16_t port)
{
  eepromWriteUnsignedInt(EEPROM_ADDR, EEPROM_UNSIGNED_INT_ADDRESS(EEPROM_MQTT_PORT_INDEX), (unsigned int)port);
  mqttPort = port;
  restartMqtt();
}

uint16_t getMqttPort()
{
  return mqttPort;
}

void setMqttUsername(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_USERNAME_ADDRESS, EEPROM_MQTT_USERNAME_LEN, str);
  strncpy(mqttUsername, str, EEPROM_MQTT_USERNAME_LEN);
  mqttUsername[EEPROM_MQTT_USERNAME_LEN] = 0;
  restartMqtt();
}

const char* getMqttUsername()
{
  return mqttUsername;
}

void setMqttPassword(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_PASSWORD_ADDRESS, EEPROM_MQTT_PASSWORD_LEN, str);
  strncpy(mqttPassword, str, EEPROM_MQTT_PASSWORD_LEN);
  mqttPassword[EEPROM_MQTT_PASSWORD_LEN] = 0;
  restartMqtt();
}

const char* getMqttPassword()
{
  return mqttPassword;
}


void setDeviceName(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_DEVICE_NAME_ADDRESS, EEPROM_DEVICE_NAME_LEN, str);
  strncpy(deviceName, str, EEPROM_DEVICE_NAME_LEN);
  deviceName[EEPROM_DEVICE_NAME_LEN] = 0;
  restartMqtt();
  restartWifi();
}

const char* getDeviceName()
{
  return deviceName;
}

void readParameters()
{
  eepromReadString(EEPROM_ADDR, EEPROM_DEVICE_NAME_ADDRESS, EEPROM_DEVICE_NAME_LEN, deviceName, EEPROM_DEVICE_NAME_LEN + 1);
  eepromReadString(EEPROM_ADDR, EEPROM_WIFI_SSID_ADDRESS, EEPROM_WIFI_SSID_LEN, wifiSsid, EEPROM_WIFI_SSID_LEN + 1);
  eepromReadString(EEPROM_ADDR, EEPROM_WIFI_PASSWORD_ADDRESS, EEPROM_WIFI_PASSWORD_LEN, wifiPassword, EEPROM_WIFI_PASSWORD_LEN + 1);
  eepromReadString(EEPROM_ADDR, EEPROM_MQTT_HOSTNAME_ADDRESS, EEPROM_MQTT_HOSTNAME_LEN, mqttHostname, EEPROM_MQTT_HOSTNAME_LEN + 1);
  eepromReadString(EEPROM_ADDR, EEPROM_MQTT_USERNAME_ADDRESS, EEPROM_MQTT_USERNAME_LEN, mqttUsername, EEPROM_MQTT_USERNAME_LEN + 1);
  eepromReadString(EEPROM_ADDR, EEPROM_MQTT_PASSWORD_ADDRESS, EEPROM_MQTT_PASSWORD_LEN, mqttPassword, EEPROM_MQTT_PASSWORD_LEN + 1);  
  mqttPort = (uint16_t)eepromReadUnsignedInt(EEPROM_ADDR, EEPROM_UNSIGNED_INT_ADDRESS(EEPROM_MQTT_PORT_INDEX));
}

void setupParameters()
{
  readParameters();
}
