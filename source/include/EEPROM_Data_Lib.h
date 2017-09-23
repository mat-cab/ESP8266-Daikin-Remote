#ifndef EEPROM_DATA_LIB_H
#define EEPROM_DATA_LIB_H

#include "Arduino.h"
#include "Measurement.h"

// EEPROM header page size (in bytes)
// Header structure is the following:
// Bytes 0 to 1: Measurement counter
// Bytes 2 to 3: Size of the schedule in its page
#define EEPROM_HEADER_SIZE 32  
// Scheduler page size in EEPROM (in byte)
#define SCHEDULER_PAGE_SIZE 512
// Measurement page size in EEPROM (in byte)
#define MEASUREMENT_PAGE_SIZE 131072 
// Half of the EEPROM remains empty...

void resetEEPROM();
void resetMeasurementEEPROM();

uint16_t readEEPROMMeasurementCounter();

void writeEEPROMMeasurementCounter(uint16_t newCounter);

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);

uint16_t readEEPROMEndOfScheduleCounter();

void writeEEPROMEndOfScheduleCounter(uint16_t newCounter);

void writeScheduleInEEPROM(Action * schedule);

void readScheduleFromEEPROM(Action ** schedule);
#endif
