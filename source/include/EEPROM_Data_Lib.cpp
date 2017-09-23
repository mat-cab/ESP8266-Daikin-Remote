#include "Arduino.h"

#include "EEPROM_Lib.h"
#include "Measurement.h"
#include "Scheduler/Action.h"
#include "EEPROM_Data_Lib.h"

void resetEEPROM() {
  // Reset the EEPROM counters to 0
  resetMeasurementEEPROM();
  writeEEPROMEndOfScheduleCounter(0);
}

void resetMeasurementEEPROM() {
  writeEEPROMMeasurementCounter(0);
}

/******************* 
 * Measurement part
 *******************/

uint16_t readEEPROMMeasurementCounter() {
  uint16_t counter = 0;

  // Read at address 0 the counter
  readEEPROM(0, (byte*)&counter, sizeof(uint16_t));

  // Return the value
  return counter;
}

void writeEEPROMMeasurementCounter(uint16_t newCounter) {
  // Write value at address 0
  writeEEPROM( 0, (byte*)&newCounter, sizeof(uint16_t));
}

void writeMeasurementInEEPROM(struct measurement *measureDatastore) {
  // First read the counter
  uint16_t counter = readEEPROMMeasurementCounter();

  // Write in the next available slot and increase counter
  writeEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + counter++ * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));

  // Update the counter
  writeEEPROMMeasurementCounter(counter);
}

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore) {
  // read the data
  readEEPROM( EEPROM_HEADER_SIZE + SCHEDULER_PAGE_SIZE + measurementIndex * sizeof(measurement), (byte*)measureDatastore, sizeof(measurement));
}

/******************* 
 * Action part
 *******************/

uint16_t readEEPROMEndOfScheduleCounter() {
  uint16_t counter = 0;

  readEEPROM(sizeof(uint16_t), (byte*)&counter, sizeof(uint16_t));

  // Return the value
  return counter;
}

void writeEEPROMEndOfScheduleCounter(uint16_t newCounter) {
  // Write value at appropriate address
  writeEEPROM(sizeof(uint16_t), (byte*)&newCounter, sizeof(uint16_t));
}

void writeScheduleInEEPROM(Action * schedule) {
  Action * currentAction = schedule;
  uint16_t counter = 0;
  // Max size for an action : 64 bytes
  uint8_t dataBuffer[32];
  uint8_t actionSize = 0;

  while ( currentAction != NULL ) {
    actionSize = sizeof(TimeMask) + sizeof(ActionMask) + currentAction->getActionMask()->getSecondaryData();

    memcpy(dataBuffer, currentAction->getActionMask()->getRawData(), sizeof(ActionMask));
    memcpy(dataBuffer + sizeof(ActionMask), currentAction->getTimeMask()->getRawData(), sizeof(TimeMask));
    //TODO: Fill for the action arguments
//    memcpy(dataBuffer + sizeof(ActionMask) + sizeof(TimeMask), #######, currentAction->getActionMask()->getSecondaryData());

    writeEEPROM( EEPROM_HEADER_SIZE + counter, (byte*)dataBuffer, actionSize);

    counter += actionSize;

    currentAction = currentAction->getNextAction();
  }

  // Update the counter
  writeEEPROMEndOfScheduleCounter(counter);
}

void readScheduleFromEEPROM(Action **schedule) {
  uint16_t EndOfSchedule = readEEPROMEndOfScheduleCounter();
  uint16_t counter = 0;

  uint8_t dataBuffer[32];

  Action *newAction, *currentAction;

  while (counter < EndOfSchedule) {
    // Always read more then necessary
    readEEPROM( EEPROM_HEADER_SIZE + counter, (byte*) dataBuffer, sizeof(dataBuffer));

    // Create a new action with the appropriate TimeMask and ActionMask
    newAction = new Action(new TimeMask(dataBuffer+sizeof(ActionMask)),new ActionMask(dataBuffer));

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
