#include <string>
#include <iostream>

#include <FS.h>
#include <Wire.h>
#include <Arduino.h>

#include "cli.hpp"
#include "mqtt.hpp"
#include "eeprom.hpp"
#include "switches.hpp"
#include "web_server.hpp"
#include "parameters.hpp"
#include "peripherals.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  Wire.begin();
  if(SPIFFS.begin())
    Serial.println("SPIFFS Initialize....ok");
  else
    Serial.println("SPIFFS Initialization...failed");
  setupParameters();
  setupPeripherals();
  setupCli();
  setupSwitch();
  setupMqtt();
  setupWebServer();
  Serial.println("Initialization finished. Starting main loop");
  
}

void loop()
{
  cliLoop();
  mqttClient.loop();
  switchesLoop();
  webServerLoop();

  static long lastCheck = 0;
  long now = millis();
  if (now < lastCheck or now - lastCheck > 2000)
  {
    lastCheck = now;

    mqttConnectionCheck();
  }
  delay(10);
}
