#include "Arduino.h"

#include "EEPROM_Lib.h"
#include "../Measurement.h"
#include "../Scheduler/Action.h"
#include "../Scheduler/ActionStaticFunctions.h"

#include "EEPROM_Header.h"

#include "ExPROM.h"

// TODO: Introduce a new counter for the measurements (to have a start and an end)

// Verify that the size of the hader is less than the allowed space
static_assert( sizeof(EEPROM_Header) <= EEPROM_HEADER_SIZE, "EEPROM Header is bigger than the allowed size");

EEPROM_Header * eeHeader;

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
  readEEPROM(0, (byte*)&eeHeader, sizeof(EEPROM_Header));
}

void writeEEPROMHeader() {
  writeEEPROM(0, (byte*)&eeHeader, sizeof(EEPROM_Header));
}

void checkEEPROMHeader() {
  if (eeHeader == NULL) {
    readEEPROMHeader();
  }
}

/******************* 
 * Measurement part
 *******************/
uint16_t readEEPROMMeasurementCounter() {
  checkEEPROMHeader();

  // Return the value
  return eeHeader->getMeasurementIndex();
}

void writeEEPROMMeasurementCounter(uint16_t newCounter) {
  checkEEPROMHeader();

  eeHeader->setMeasurementIndex(newCounter);
}

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  checkEEPROMHeader();

  // Write in the next available slot and increase counter
  writeEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + eeHeader->increaseMeasurementIndex() * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  // read the data
  readEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + measurementIndex * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
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
