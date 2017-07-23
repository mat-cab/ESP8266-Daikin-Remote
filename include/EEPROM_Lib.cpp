#include "Arduino.h"
#include "Wire.h"
#include "I2C_Lib.h"
#include "Debug_Lib.h"
#include "EEPROM_Lib.h"

void writeEEPROM(uint16_t eeaddress, byte *data, uint8_t dataLength ) {
  uint8_t writtenData = 0, dataWriteLength = 0;
  byte returnCode;  

  // Loop for all the data
  while (writtenData < dataLength) {
    // This is the next write length
    // reduce to less than 30 bytes
    dataWriteLength = dataLength > ESP8266_WIRE_BUFFER_LENGTH-2 ? ESP8266_WIRE_BUFFER_LENGTH-2 : dataLength;
    // stay on the same memory page
    dataWriteLength = EEPROM_PAGE_SIZE - (eeaddress % EEPROM_PAGE_SIZE) < dataWriteLength ? EEPROM_PAGE_SIZE - (eeaddress % EEPROM_PAGE_SIZE) : dataWriteLength;

    // Begin writing into EEPROM
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(eeaddress >> 8));   // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    for (byte i=0; i<dataWriteLength; i++) {
      Wire.write(*(data+i));
    }

    returnCode = Wire.endTransmission();

    // Analyse error
    if (returnCode > 0) {
      debug("Error "+String(returnCode)+" when writing to the EEPROM");
    }
    // update the number of data written
    writtenData += dataWriteLength;

    // update the EEPROM and data addresses
    eeaddress += dataWriteLength;
    data += dataWriteLength;

    // wait for the write to complete
    delay(EEPROM_DELAY);
  }
}
 
void readEEPROM(uint16_t eeaddress, byte* dataPointer, uint8_t dataLength) {
  // if requesting more than 32 bytes, necessary to read the first 32 bytes because of Wire library restriction
  if (dataLength > ESP8266_WIRE_BUFFER_LENGTH) {  
    // read first 32 bytes
    readEEPROM(eeaddress, dataPointer, ESP8266_WIRE_BUFFER_LENGTH);
    // read the rest
    readEEPROM(eeaddress+ESP8266_WIRE_BUFFER_LENGTH, dataPointer+ESP8266_WIRE_BUFFER_LENGTH, dataLength-ESP8266_WIRE_BUFFER_LENGTH);

    // Do not execute this read
    return;
  }

  byte returnCode;
   
  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write((uint8_t)(eeaddress >> 8));   // MSB
  Wire.write((uint8_t)(eeaddress & 0xFF)); // LSB
  returnCode = Wire.endTransmission();

  if (returnCode > 0) {
    debug("Error "+String(returnCode)+" when reading from the EEPROM");
  }
 
  Wire.requestFrom((uint8_t)EEPROM_ADDR, dataLength);

  returnCode = Wire.available();

  if (returnCode != dataLength) {
    debug("Error during reading from the EEPROM. "+String(dataLength)+" bytes were requested, but only "+String(returnCode)+" are available");
  }

  for (byte i=0; i<dataLength;i++) {
      if (Wire.available()){
        *(dataPointer+i) = Wire.read();
      }
  }
}
