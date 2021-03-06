#include "Time.h"

#include "../Debug_Lib.h"
#include "../RTCMem_Lib.h"
#include "../ExPROM/ExPROM.h"
#include "../Wifi/Wifi.h"

#include "ActionStaticFunctions.h"

#include "Scheduler.h"

Action * schedule;
Action * nextActionOnSchedule;
Scheduler_RTCData * schedulerRTCData;
uint8_t lastDayOfExecution;

void initializeScheduler() {
  debug("Starting initialization of scheduler..."); 

  // read the data from RTC Memory
  schedulerRTCData = getRTCPointer_schedulerRTCData();

  // Cast it to appropriate data
  schedule = schedulerRTCData->getAction();
  lastDayOfExecution = schedulerRTCData->getLastDayOfExecution();

  debug("Scheduler initialization finished");
}

bool resetScheduler(bool hardReset) {
  bool error = false;

  debug("Resetting the scheduler");

  if (hardReset) {
    // set the scheduler to empty
    schedule = NULL;
  }

  // Try to connect to the wifi
  if (!connectToWifi()) {
    // in case no connection 
    debug("Could not connect to wifi during scheduler hard reset");
    // save error
    error = true;
  }

  // Try to receive the schedule
  if (!error && !receiveWifi(&schedule)) {
    // If asked for a hard reset (initial reset only), but impossible to reach the schedule server, then go to sleep
    debug("Could not update the schedule after the scheduler hard reset!");
    // save error
    error = true;
  }

  // If there was an error during a hard reset
  if (error && hardReset) {
    // Warn about the following deepSleep
    debug("Sleeping for "+String(SCHEDULER_RESET_ERROR_SLEEP)+" ms, hoping connection will be OK then.");

    // close the debug to make sure the messages are sent
    endDebug();

    // go to sleep
    ESP.deepSleep(SCHEDULER_RESET_ERROR_SLEEP*1000);
  } else if (!error) {
    // sort the schedule
    sortSchedule();

    // write in EEPROM
    writeScheduleInEEPROM(schedule); 

    // Also update the last day of execution
    lastDayOfExecution = weekday();

    // save in RTC Memory
    saveSchedulerInRTCMem();

    debug("Scheduler reset finished");
  }

  return isSchedulerActive();
}

void clearScheduler() {
  schedule = NULL;
}

void runScheduler() {
  debug("Running schedule!");

  if (!isSchedulerActive()) {
    // If scheduler is not active, then do nothing
    return;
  }

  bool readFromEEPROM = false;

  // Verify if the day has changed since the last execution
  if (lastDayOfExecution != weekday()) {
    // Send a message to debug
    debug("Resetting the execution flag on all actions");

    // if so, read the schedule from the EEPROM
    readScheduleFromEEPROM(&schedule);

    // set the flag to already read
    readFromEEPROM = true;  

    // reset all actions executed
    schedule->resetAllExecuted();  

    // and save the day
    lastDayOfExecution = weekday();

    // and write the schedule in EEPROM
    writeScheduleInEEPROM(schedule);
 
    debug("Flag reset");
  }
 
  // Verify if the first action is to be executed
  if ( !(schedule->getSecondsFromNow() <= 0) ) {
    // Do nothing for this cycle
    debug("First action is not to be executed");
  } else {
    // Verify that we haven't read yet the schedule
    if (!readFromEEPROM) {
      debug("Reading schedule from EEPROM..."); 
      // read the whole schedule
      readScheduleFromEEPROM(&schedule);
    }

    // print it
    printSchedule();

    // Loop for all actions to be executed
    do {
      // Execute the action
      schedule->run();
 
      // sort the actions in the schedule
      sortSchedule();
    } while (schedule->getSecondsFromNow() <= 0);

    // Write in EEPROM once all actions are executed
    writeScheduleInEEPROM(schedule); 
  }

  // Save the scheduler information in the RTC and in the EEPROM
  saveSchedulerInRTCMem();

  // Send debug message
  debug("Schedule was run succesfully");
}

void saveSchedulerInRTCMem() {
  schedulerRTCData->updateLastDayOfExecution(lastDayOfExecution);
  schedulerRTCData->updateActionData(schedule);
}

void sortSchedule() {
  // if schedule is empty, do nothing
  if (!isSchedulerActive()) {
    return;
  }

  // If there is only one action, then the schedule is sorted
  if (schedule->isLastAction()) {
    return;
  }

  // If there is two or more actions
  Action * firstAction = schedule;
  Action * secondAction = firstAction->getNextAction();
  // Verify if the second one is before after the first one
  if (secondAction->getSecondsFromNow() < schedule->getSecondsFromNow() ) {
    // The new schedule starts with the second action
    schedule = secondAction;

    // In this new schedule, add the old first action
    schedule->addAction( firstAction );
  }
}

bool isSchedulerActive() {
  return (schedule != NULL);
}

uint64_t remainingTimeBeforeNextAction() {
  // First option : Scheduler is not strict
  #ifndef SCHEDULER_STRICT
  // Always return forever
  return UINT64_MAX;

  #else
  // Return the time to the next action
  return schedule->getSecondsFromNow();

  #endif
}


void printSchedule() {
  uint8_t i = 1;
  
  if (schedule == NULL) {
    debug("Schedule is empty!");

    return;
  }

  Action *nextAction = schedule;

  do {
    debug("Schedule place "+String(i++)+":");
    nextAction->print();
    debug("Executed in "+String(nextAction->getSecondsFromNow())+" seconds");
    
    nextAction = nextAction->getNextAction();
  } while (nextAction != NULL);
}

void parseNewAction(String actionString) {
  Action * newAction = parseActionFromString( actionString );

  if (newAction == NULL) {
    debug("Action not added to the schedule");
  } else {
    // Check whether there is already at least one action
    if (schedule == NULL) {
      schedule = newAction;
    } else {
      // if so, add to the schedule
      schedule->addAction(newAction);
    }
    debug("Action added to the schedule");
  }
}

