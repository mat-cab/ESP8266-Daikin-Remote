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
// The rest of the EEPROM is dedicated to the measurement storage
// this is the total size of the EEPROM
#define EEPROM_TOTAL_SIZE 262144

// Maximum Measurement index in EEPROM
#define EEPROM_MAX_MEASUREMENT_INDEX (EEPROM_TOTAL_SIZE - EEPROM_HEADER_SIZE - SCHEDULER_PAGE_SIZE) / sizeof(measurement)

void resetEEPROM();
void writeEEPROM();

// Header operations
void readEEPROMHeader();
void writeEEPROMHeader();
void checkEEPROMHeader();

// Measurement operations
uint16_t readEEPROMMeasurementCounter();

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);

void deleteMeasurementsFromEEPROM( uint16_t total );

// Schedule opeartions
uint16_t readEEPROMEndOfScheduleCounter();

void writeEEPROMEndOfScheduleCounter(uint16_t newCounter);

void writeScheduleInEEPROM(Action * schedule);

void readScheduleFromEEPROM(Action ** schedule);
#endif
