#include "Arduino.h"

#include "EEPROM_Lib.h"
#include "../Measurement.h"
#include "../Scheduler/Action.h"
#include "../Scheduler/ActionStaticFunctions.h"

#include "EEPROM_Header.h"

#include "ExPROM.h"

// Verify that the size of the hader is less than the allowed space
static_assert( sizeof(EEPROM_Header) <= EEPROM_HEADER_SIZE, "EEPROM Header is bigger than the allowed size");

EEPROM_Header * eeHeader = NULL;

void resetEEPROM() {
  // read the header if necessary
  checkEEPROMHeader();

  // reset the header
  eeHeader->reset();
}

void writeEEPROM() {
  writeEEPROMHeader();
}

/*********************
 * Header operations
 *********************/
void readEEPROMHeader() {
  readEEPROM(0, (byte*)eeHeader, sizeof(EEPROM_Header));
}

void writeEEPROMHeader() {
  writeEEPROM(0, (byte*)eeHeader, sizeof(EEPROM_Header));
}

void checkEEPROMHeader() {
  if (eeHeader == NULL) {
    eeHeader = new EEPROM_Header();

    readEEPROMHeader();
  }
}

/******************* 
 * Measurement part
 *******************/
uint16_t readEEPROMMeasurementCounter() {
  checkEEPROMHeader();

  // Return the value
  if (eeHeader->getMeasurementIndexEnd() >= eeHeader->getMeasurementIndexStart()) {
    return (eeHeader->getMeasurementIndexEnd() - eeHeader->getMeasurementIndexStart());
  } else {
    // case of write over
    return (eeHeader->getMeasurementIndexEnd() + (EEPROM_MAX_MEASUREMENT_INDEX - eeHeader->getMeasurementIndexStart()));
  }
}

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  checkEEPROMHeader();

  // Write in the next available slot and increase counter
  writeEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + eeHeader->getMeasurementIndexEnd() * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));

  // Verify if the page is full
  if (eeHeader->increaseMeasurementIndexEnd() >= EEPROM_MAX_MEASUREMENT_INDEX) {
    // If it is, loop back to the start of the page
    eeHeader->resetMeasurementIndexEnd();
  }
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  checkEEPROMHeader();

  // read the data
  if (eeHeader->getMeasurementIndexEnd() >= eeHeader->getMeasurementIndexStart() || (eeHeader->getMeasurementIndexStart() + measurementIndex) < EEPROM_MAX_MEASUREMENT_INDEX ) {
    readEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + (eeHeader->getMeasurementIndexStart() + measurementIndex) * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
  } else {
    uint16_t index = measurementIndex - (EEPROM_MAX_MEASUREMENT_INDEX - eeHeader->getMeasurementIndexStart());

    readEEPROM(EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + index * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
  }
}

void deleteMeasurementsFromEEPROM( uint16_t total ) {
  checkEEPROMHeader();
  
  uint16_t lastStart = eeHeader->getMeasurementIndexStart();
  uint16_t newStart = eeHeader->increaseMeasurementIndexStart( total ); 

  // Increase the start counter of the measurements (only the first will be deleted)
  if ( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + newStart * sizeof(measurement) >= EEPROM_TOTAL_SIZE ) {
    uint16_t rollOver = total - ((EEPROM_TOTAL_SIZE - EEPROM_HEADER_SIZE - SCHEDULER_PAGE_SIZE) / sizeof(measurement) - lastStart);

    eeHeader->resetMeasurementIndexStart(rollOver);
  }
}

/******************* 
 * Action part
 *******************/

uint16_t readEEPROMEndOfScheduleCounter() {
  checkEEPROMHeader();

  // Return the value
  return eeHeader->getActionIndex();
}

void writeEEPROMEndOfScheduleCounter(uint16_t newCounter) {
  checkEEPROMHeader();

  eeHeader->setActionIndex(newCounter);
}

void writeScheduleInEEPROM(Action * schedule) {
  Action * currentAction = schedule;
  // Max size for an action : 32 bytes
  uint8_t dataBuffer[32];
  uint8_t actionSize = 0;

  checkEEPROMHeader();

  // reset the actionIndex
  eeHeader->resetActionIndex();

  while ( currentAction != NULL ) {
    actionSize = sizeof(TimeMask) + sizeof(ActionMask) + currentAction->getActionMask()->getSecondaryData();

    memcpy(dataBuffer, currentAction->getActionMask()->getRawData(), sizeof(ActionMask));
    memcpy(dataBuffer + sizeof(ActionMask), currentAction->getTimeMask()->getRawData(), sizeof(TimeMask));

    // If there are somme additional data, save it too
    if (currentAction->getActionMask()->getSecondaryData() > 0 ) {
      memcpy(dataBuffer + sizeof(ActionMask) + sizeof(TimeMask), currentAction->getAdditionalData()->getRawData(), currentAction->getActionMask()->getSecondaryData());
    }

    writeEEPROM( EEPROM_HEADER_SIZE + eeHeader->getActionIndex(), (byte*)dataBuffer, actionSize);

    eeHeader->increaseActionIndex(actionSize);

    currentAction = currentAction->getNextAction();
  }
}

void readScheduleFromEEPROM(Action **schedule) {
  uint16_t counter = 0;
  uint8_t dataBuffer[32];

  Action *newAction, *currentAction;

  checkEEPROMHeader();

  while (counter < eeHeader->getActionIndex()) {
    // Always read more then necessary
    readEEPROM( EEPROM_HEADER_SIZE + counter, (byte*) dataBuffer, sizeof(dataBuffer));

    // Create a new action with the appropriate TimeMask and ActionMask
    newAction = getNewAction(new TimeMask(dataBuffer+sizeof(ActionMask)),new ActionMask(dataBuffer), dataBuffer+sizeof(ActionMask)+sizeof(TimeMask));

    // If it is the first time we read (counter is empty), then update the schedule variable
    if (counter == 0) {
      *schedule = newAction;
      currentAction = newAction;
    } else {
      // Otherwise add after the current action
      currentAction->addAction(newAction);
      // and continue with the next action
      currentAction = currentAction->getNextAction();
    }
  
    counter += sizeof(ActionMask) + sizeof(TimeMask) + newAction->getActionMask()->getSecondaryData();
  }
}
