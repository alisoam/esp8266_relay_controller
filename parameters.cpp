#include "parameters.hpp"

#include <cstring>

#include "eeprom.hpp"

# ifndef EEPROM_ADDR
#   define EEPROM_ADDR (0x50)
# endif

#define EEPROM_DEVICE_NAME_LEN    30
#define EEPROM_WIFI_SSID_LEN      30
#define EEPROM_WIFI_PASSWORD_LEN  30
#define EEPROM_MQTT_HOSTNAME_LEN  30
#define EEPROM_MQTT_USERNAME_LEN  30
#define EEPROM_MQTT_PASSWORD_LEN  30

#define EEPROM_START_ADDRESS  100
#define EEPROM_UNSIGNED_INT_START_ADDRESS   800

#define EEPROM_DEVICE_NAME_ADDRESS      EEPROM_START_ADDRESS  
#define EEPROM_WIFI_SSID_ADDRESS      (EEPROM_DEVICE_NAME_ADDRESS    + EEPROM_DEVICE_NAME_LEN)
#define EEPROM_WIFI_PASSWORD_ADDRESS  (EEPROM_WIFI_SSID_ADDRESS      + EEPROM_WIFI_SSID_LEN)
#define EEPROM_MQTT_HOSTNAME_ADDRESS  (EEPROM_WIFI_PASSWORD_ADDRESS  + EEPROM_WIFI_PASSWORD_LEN)
#define EEPROM_MQTT_USERNAME_ADDRESS  (EEPROM_MQTT_HOSTNAME_ADDRESS  + EEPROM_MQTT_HOSTNAME_LEN)
#define EEPROM_MQTT_PASSWORD_ADDRESS  (EEPROM_MQTT_USERNAME_ADDRESS  + EEPROM_MQTT_USERNAME_LEN)

#define EEPROM_MQTT_PORT_INDEX              100
#define EEPROM_UNSIGNED_INT_ADDRESS(index)  (EEPROM_UNSIGNED_INT_START_ADDRESS + index * EEPROM_UNSINGED_INT_SIZE)

//
static char deviceName[EEPROM_DEVICE_NAME_LEN + 1];

static char wifiSsid[EEPROM_WIFI_SSID_LEN + 1];
static char wifiPassword[EEPROM_WIFI_PASSWORD_LEN + 1];

static char mqttHostname[EEPROM_MQTT_HOSTNAME_LEN + 1];
static char mqttUsername[EEPROM_MQTT_USERNAME_LEN + 1];
static char mqttPassword[EEPROM_MQTT_PASSWORD_LEN + 1];

unsigned int mqttPort = 0;

// WIFI

void setWifiSsid(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_WIFI_SSID_ADDRESS, EEPROM_WIFI_SSID_LEN, str);
}

const char* getWifiSsid()
{
  return wifiSsid;
}

void setWifiPassword(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_WIFI_PASSWORD_ADDRESS, EEPROM_WIFI_PASSWORD_LEN, str);
}

const char* getWifiPassword()
{
  return wifiPassword;
}

// MQTT

void setMqttHostname(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_HOSTNAME_ADDRESS, EEPROM_MQTT_HOSTNAME_LEN, str);
}

const char* getMqttHostname()
{
  return mqttHostname;
}


void setMqttPort(uint16_t port)
{
  eepromWriteUnsignedInt(EEPROM_ADDR, EEPROM_UNSIGNED_INT_ADDRESS(EEPROM_MQTT_PORT_INDEX), (unsigned int)port);
}

uint16_t getMqttPort()
{
  return mqttPort;
}

void setMqttUsername(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_USERNAME_ADDRESS, EEPROM_MQTT_USERNAME_LEN, str);
}

const char* getMqttUsername()
{
  return mqttUsername;
}

void setMqttPassword(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_MQTT_PASSWORD_ADDRESS, EEPROM_MQTT_PASSWORD_LEN, str);
}

const char* getMqttPassword()
{
  return mqttPassword;
}


void setDeviceName(const char* str)
{
  eepromWriteString(EEPROM_ADDR, EEPROM_DEVICE_NAME_ADDRESS, EEPROM_DEVICE_NAME_LEN, str);
}

const char* getDeviceName()
{
  return deviceName;
}

// setup
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
