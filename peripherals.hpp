#ifndef PERIPHERALS_HPP
#define PERIPHERALS_HPP

#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>

extern RTC_DS1307 rtc;
extern WiFiClient espClient;
extern WiFiServer wifiServer;

void restartWifi();
void restartAp();
void setupPeripherals();
void wifiLoop();

#endif
