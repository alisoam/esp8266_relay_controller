#include <cstdio>
#include <string>

#include "eeprom.h"

# ifndef EEPROM_ADDR
#   define EEPROM_ADDR (0x50)
# endif

#define SWITCHES_NUMBER 4

void setup()
{
  cliSetup();
  setupSwitch();
  setupMqtt();
  char buffer1[6] = "abcde";
  i2c_eeprom_write_page(EEPROM_ADDR, 0, (uint8_t*)buffer1, 6);
  delay(500);
  char buff[6];
  i2c_eeprom_read_buffer(EEPROM_ADDR,0, (uint8_t*)buff, 6);
  buff[5] = 0;
  Serial.print("EEPROM: ");
  Serial.println(buff);
  
  Serial.println("initialization finished. starting main loop");
}


void loop()
{
  mqttClient.loop();
  commandLineLoop();

  static long lastCheck = 0;
  long now = millis();
  if (now < lastCheck or now - lastCheck > 2000)
  {
    lastCheck = now;

    mqttConnectionCheck();
    
    static int loopCounter = 0;
    char buffer[20];
    snprintf(buffer, 20, "loop %d", loopCounter++);
    Serial.println(buffer);  
  }
}
