#ifndef EEPROM_H
#define EEPROM_H

#include <cstdint>

#ifndef EEPROM_PAGE_SIZE
# define EEPROM_PAGE_SIZE 32U 
#endif

#ifndef EEPROM_MAX_LEN
# define EEPROM_MAX_LEN 30U
#endif

#ifndef EEPROM_UNSINGED_INT_SIZE
# define EEPROM_UNSINGED_INT_SIZE sizeof(unsigned int)
#endif

#ifndef EEPROM_BOOL_SIZE
# define EEPROM_BOOL_SIZE sizeof(char)
#endif

void eepromWriteByte(int deviceaddress, unsigned int eeaddress, uint8_t data);
uint8_t eepromReadByte( int deviceaddress, unsigned int eeaddress);

void eepromWrite(int deviceaddress, unsigned int eeaddress, uint8_t* buffer, unsigned int length);
void eepromRead(int deviceaddress, unsigned int eeaddress, uint8_t *buffer, unsigned int length);

void eepromReadString(int deviceaddress, unsigned int eeaddress, unsigned int len, char* str, unsigned int bufferLen);
void eepromWriteString(int deviceaddress, unsigned int eeaddress, unsigned int len, const char* str);

unsigned int eepromReadUnsignedInt(int deviceaddress, unsigned int eeaddress);
void eepromWriteUnsignedInt(int deviceaddress, unsigned int eeaddress, unsigned int value);

bool eepromReadBool(int deviceaddress, unsigned int eeaddress);
void eepromWriteBool(int deviceaddress, unsigned int eeaddress, bool value);

#endif
