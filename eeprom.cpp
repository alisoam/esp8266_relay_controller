#include "eeprom.hpp"

#include <algorithm>

#include <Arduino.h>
#include <Wire.h>

void eepromWriteByte(int deviceaddress, unsigned int eeaddress, uint8_t data)
{
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));
  Wire.write((int)(eeaddress & 0xFF));
  Wire.write(rdata);
  Wire.endTransmission();
  delay(10);
}

void eepromWrite(int deviceaddress, unsigned int eeaddress, uint8_t* buffer, unsigned int length)
{
  unsigned int writtenLength = 0;
  while (writtenLength < length)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)((eeaddress + writtenLength) >> 8)); // MSB
    Wire.write((int)((eeaddress + writtenLength) & 0xFF)); // LSB
    unsigned int currentWriteLength = std::min(length - writtenLength, EEPROM_MAX_LEN);
    currentWriteLength = std::min(currentWriteLength, EEPROM_PAGE_SIZE - (eeaddress + writtenLength) % EEPROM_PAGE_SIZE);
    for (unsigned int c = 0; c < currentWriteLength; c++)
      Wire.write(buffer[writtenLength + c]);
    Wire.endTransmission();
    writtenLength += currentWriteLength;
    delay(10);
  }
}

uint8_t eepromReadByte(int deviceaddress, unsigned int eeaddress)
{
  uint8_t rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available())
    rdata = Wire.read();
  delay(10);
  return rdata;
}

void eepromRead(int deviceaddress, unsigned int eeaddress, uint8_t *buffer, unsigned int length)
{
  unsigned int readLength = 0;
  while (readLength < length)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)((eeaddress + readLength) >> 8)); // MSB
    Wire.write((int)((eeaddress + readLength) & 0xFF)); // LSB
    Wire.endTransmission();
    unsigned int currentReadLength = std::min(length - readLength, EEPROM_MAX_LEN);
    Wire.requestFrom(deviceaddress, currentReadLength);
    for (unsigned int c = 0; c < currentReadLength; c++)
      if (Wire.available())
        buffer[readLength + c] = Wire.read();
      else
        return;
    readLength += currentReadLength;
    delay(10);
  }
}

void eepromReadString(int deviceaddress, unsigned int eeaddress, unsigned int len, char* buffer, unsigned int bufferLen)
{
  auto readLen = std::min(len, bufferLen - 1);
  eepromRead(deviceaddress, eeaddress, (uint8_t*)buffer, readLen);
  buffer[readLen] = '\0';
}

void eepromWriteString(int deviceaddress, unsigned int eeaddress, unsigned int len, const char* str)
{
  auto writeLen = std::min(strlen(str) + 1, EEPROM_MAX_LEN);
  writeLen = std::min(writeLen, len);
  eepromWrite(deviceaddress, eeaddress, (uint8_t*)str, writeLen);
}

unsigned int eepromReadUnsignedInt(int deviceaddress, unsigned int eeaddress)
{
  unsigned int toBeReturned;
  eepromRead(deviceaddress, eeaddress, (uint8_t*)&toBeReturned, EEPROM_UNSINGED_INT_SIZE);
  return toBeReturned;
}

void eepromWriteUnsignedInt(int deviceaddress, unsigned int eeaddress, unsigned int value)
{
  eepromWrite(deviceaddress, eeaddress, (uint8_t*)&value, EEPROM_UNSINGED_INT_SIZE);
}

bool eepromReadBool(int deviceaddress, unsigned int eeaddress)
{
  char buffer;
  eepromRead(deviceaddress, eeaddress, (uint8_t*)&buffer, 1);
  if (buffer == 0xaa)
    return true;
  return false;
}

void eepromWriteBool(int deviceaddress, unsigned int eeaddress, bool value)
{
  eepromWrite(deviceaddress, eeaddress, (uint8_t*)(value? "\xaa":"\x00"), EEPROM_BOOL_SIZE);
}
