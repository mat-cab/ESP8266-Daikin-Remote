#include "Arduino.h"
#include "EEPROM_Lib.h"
#include "Measurement.h"

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  // First read the counter
  uint16_t counter;
  readEEPROM(0, (byte*)&counter, sizeof(uint16_t));  
  // Write in the next available slot
  writeEEPROM( sizeof( uint16_t ) + counter++ * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
  // Update the counter
  writeEEPROM( 0, (byte*)&counter, sizeof(uint16_t));
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  readEEPROM(sizeof( uint16_t ) + measurementIndex * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
}
