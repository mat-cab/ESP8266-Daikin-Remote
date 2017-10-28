#ifndef EXPROM_H_
#define EXPROM_H_

#include "Arduino.h"

#include "../Measurement.h"
#include "../Scheduler/Action.h"

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
void writeEEPROM();

// Header operations
void readEEPROMHeader();
void writeEEPROMHeader();
void checkEEPROMHeader();

// Measurement operations
uint16_t readEEPROMMeasurementCounter();

void writeEEPROMMeasurementCounter(uint16_t newCounter);

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);

// Schedule opeartions
uint16_t readEEPROMEndOfScheduleCounter();

void writeEEPROMEndOfScheduleCounter(uint16_t newCounter);

void writeScheduleInEEPROM(Action * schedule);

void readScheduleFromEEPROM(Action ** schedule);
#endif
