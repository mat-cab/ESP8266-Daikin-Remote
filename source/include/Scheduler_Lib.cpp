#include "Time.h"

#include "Debug_Lib.h"
#include "RTCMem_Lib.h"

#include "Scheduler_Lib.h"

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

void resetScheduler() {
  debug("Resetting the scheduler");

  // Fill with stuff initially
  Action * newAction = new Action(AC_START, 0x01, 17, 30, 0);
  schedule = newAction;

  newAction = new Action(Action(AC_STOP, 0x7F, 15, 30, 0));
  schedule->addAction(newAction);

  newAction = new Action(Action(AC_STOP, 0x7F, 23, 59, 59));
  schedule->addAction(newAction);

  newAction = new Action(Action(AC_STOP, 0x08, 20, 0, 0));
  schedule->addAction(newAction);

  debug("All actions added!");

  // sort the schedule
  sortSchedule();

  debug("Scheduler reset finished");
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

  // Verify if the day has changed since the last execution
  if (lastDayOfExecution != weekday()) {
    // if so, reset all actions executed
    schedule->resetAllExecuted();  

    // and save the day
    lastDayOfExecution = weekday();
  }
 
  // Verify if the first action is to be executed
  if ( !(schedule->getSecondsFromNow() <= 0) ) {
    // Do nothing for this cycle
    debug("First action is not to be executed");
    return;
  }

  //TODO: Read the full schedule from the EEPROM Mem

  // Loop for all actions to be executed
  do {
    // Execute the action
    schedule->run();

    // sort the actions in the schedule
    sortSchedule();
  } while (schedule->getSecondsFromNow() <= 0);

  // Save the scheduler information in the RTC and in the EEPROM
  saveSchedulerInRTCMem();
  //TODO: Save the updated schedule in the EEPROM Mem

  // Send debug message
  debug("Schedule was run succesfully");
}

void saveSchedulerInRTCMem() {
  schedulerRTCData->updateLastDayOfExecution(lastDayOfExecution);
  schedulerRTCData->updateActionData(schedule);
}

void sortSchedule() {
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
