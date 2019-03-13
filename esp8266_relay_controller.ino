#include <Wire.h>
#include <Arduino.h>

#include "cli.hpp"
#include "mqtt.hpp"
#include "switches.hpp"
#include "parameters.hpp"
#include "peripherals.hpp"

#include "eeprom.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("start");
  Wire.begin();
  setupParameters();
  setupPeripherals();
  setupCli();
  setupSwitch();
  setupMqtt();
  pinMode(9, INPUT);
  Serial.println("Initialization finished. Starting main loop");
}

void loop()
{
  cliLoop();
  mqttClient.loop();
  switchesLoop();

  static long lastCheck = 0;
  long now = millis();
  if (now < lastCheck or now - lastCheck > 2000)
  {
    lastCheck = now;
    Serial.println(digitalRead(9), HEX);
    mqttConnectionCheck();
  }
  delay(10);
}
