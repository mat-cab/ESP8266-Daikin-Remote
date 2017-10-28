#ifndef EEPROM_LIB_H_
#define EEPROM_LIB_H_

#include "Arduino.h"

// Constants for the 24LC256 memory (External ROM)
// EEPROM I2C Address
#define EEPROM_ADDR 0x50
// EEPROM delay for the write operations
#define EEPROM_DELAY 6
// EEPROM page size in bytes
#define EEPROM_PAGE_SIZE 64

void writeEEPROM(uint16_t eeaddress, byte *data, uint8_t dataLength );

void readEEPROM(uint16_t eeaddress, byte* dataPointer, uint8_t dataLength);
#endif
