#ifndef EEPROM_MEASUREMENT_Lib_H
#define EEPROM_MEASUREMENT_Lib_H

#include "Arduino.h"
#include "Measurement.h"

void resetEEPROM();

uint16_t readEEPROMCounter();

void writeEEPROMCounter(uint16_t newCounter); 

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);
#endif
