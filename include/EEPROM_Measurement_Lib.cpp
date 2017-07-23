#include "Arduino.h"

#include "EEPROM_Lib.h"
#include "Measurement.h"
#include "EEPROM_Measurement_Lib.h"

void resetEEPROM() {
  // Reset the EEPROM counter to 0
  writeEEPROMCounter(0);
}

uint16_t readEEPROMCounter() {
  uint16_t counter = 0;

  // Read at address 0 the counter
  readEEPROM(0, (byte*)&counter, sizeof(uint16_t));

  // Return the value
  return counter;
}

void writeEEPROMCounter(uint16_t newCounter) {
  // Write value at address 0
  writeEEPROM( 0, (byte*)newCounter, sizeof(uint16_t));
}

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  // First read the counter
  uint16_t counter = readEEPROMCounter();

  // Write in the next available slot and increase counter
  writeEEPROM( sizeof( uint16_t ) + counter++ * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));

  // Update the counter
  writeEEPROMCounter( counter);
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  // read the data
  readEEPROM(sizeof( uint16_t ) + measurementIndex * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
}
