#include "Time.h"

#include "Debug_Lib.h"

#include "Scheduler_Lib.h"

Action * schedule;
Action * nextActionOnSchedule;

void initializeScheduler() {
  debug("Starting initialization of scheduler..."); 

  // read the action from RTC Memory (should be an empty action)
  schedule = Action::readFromRTCMem();

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

  // Update the schedule
  updateScheduler();

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

  // select the action which was scheduled
  Action *startAction = schedule;

  // Verify if the first action is to be executed
  if (!((startAction->getDaysMask() & (1 << weekday())) && startAction->getHour() <= hour() && startAction->getMinute() <= minute() && startAction->getSecond() <= second())) {
    // First action is not to be executed
    // Do nothing for this cycle
    debug("First action is not to be executed");
    return;
  }

  // select the next new action
  Action *newerNextAction = getNextAction();

  debug("Executing all actions between the following two actions:");
  startAction->print();
  newerNextAction->print();

  // get the time between the two
  time_t timeInBetween = getSecondsBetween(startAction, newerNextAction);

  debug("There is "+String(timeInBetween)+" seconds between those two actions");


  // Execute the first action
  startAction->run();

  // Loop through all the actions (start after the current one)
  Action *currentAction = getNextActionAfter(startAction);
  debug("Next action after startAction is the following:");
  currentAction->print();
  
  // if there is only this action (there is no other actions for today!), there is nothing else to do
  if (startAction == currentAction) {
    debug("There is no other action to be executed");
  } else {
    // Otherwise there are some other actions to execute
    debug("Also executing other actions");
    time_t timeWithCurrent = getSecondsBetween(startAction, currentAction);
    // Only execute actions which are before the newerNextAction
    while (timeWithCurrent < timeInBetween) {
      // only run the current action if not already executed
      if (!currentAction->isExecuted()) {
        currentAction->run();
      }

      // Select the next action
      currentAction = getNextActionAfter(currentAction);

      // Compute the time since the start action
      timeWithCurrent = getSecondsBetween(startAction, currentAction);
      
      debug("Selected next action to be run in "+String(timeWithCurrent)+" second :");
      currentAction->print();
    }
  }

  debug("Schedule was run succesfully");

  // update the schedule
  updateScheduler(newerNextAction);
}

void saveSchedulerInRTCMem() {

}

void updateScheduler() {
  Action *nextAction = getNextAction();
  
  updateScheduler(nextAction);
}

void updateScheduler(Action *firstAction) {
  debug("Updating scheduler...");
  // If schedule is empty, it is done
  if (!isSchedulerActive()) {
    return;
  }

  // If the first action is already the wanted first action, do nothing
  if (firstAction == schedule) {
    return;
  }

  Action *initialStart = schedule;
  Action *currentAction = initialStart;
  
  // Loop through all actions
  while (currentAction != NULL) {
    // if the next action is the new start
    if (currentAction->getNextAction() == firstAction) {
      // make this action the last one
      currentAction->makeLastAction();
      // take the next one (firstAction)
      currentAction = firstAction;
    } else if (currentAction->getNextAction() == NULL ) {
      // If it is the end of the list, loop at the beginning
      currentAction->addAction(initialStart);
      // Finish at the last action
      currentAction = NULL;
    } else {
      // Go to the next action
      currentAction = currentAction->getNextAction();
    }
  }

  // set the schedule to the new first action
  schedule = firstAction;

  // save in RTC memory
  schedule->saveInRTCMem();

  debug("Scheduler updated");
}

bool isSchedulerActive() {
  return (schedule != NULL);
}

Action *getNextAction() {
  // If the schedule is empty, return NULL
  if (schedule == NULL) {
    return NULL;
  }

  // initialize the next action to the first one
  Action *currentAction = schedule;
  Action *nextAction = currentAction;
  time_t minTime = getSecondsUntil(nextAction), nextTime;

  // loop through all the actions
  while ( currentAction->getNextAction() != NULL ) {
    currentAction = currentAction->getNextAction();

    nextTime = getSecondsUntil(currentAction);

    // if next execution time is less than the min time, remember this action
    if (nextTime < minTime) {
      minTime = nextTime;
      nextAction = currentAction;
    }
  }

  // return the nextAction
  return nextAction;
}

Action *getNextActionAfter(Action *actionStart) {
  // If the schedule is empty, return NULL
  if (schedule == NULL) {
    return NULL;
  }

  // initialize the next action to the first one
  Action *currentAction = schedule;
  
  // in case there is only one action, return the current action
  if (currentAction->getNextAction() == NULL) {
    return currentAction;
  }

  Action *nextAction = NULL;
  // initialize the min time to one week
  time_t minTime = 7 * 24 * 60 * 60, nextTime;

  while ( currentAction->getNextAction() != NULL ) {
    currentAction = currentAction->getNextAction();

    nextTime = getSecondsBetween(actionStart, currentAction);

    // do not return an action which was already executed
    if (nextTime < minTime && !currentAction->isExecuted()) {
      minTime = nextTime;
      nextAction = currentAction;
    }
  }

  // return the next action
  return nextAction;
}

time_t getSecondsFrom(Action *action, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second) {
  uint8_t today = weekday, nextday;
  time_t timeLeft = (action->getHour() * 60 + action->getMinute()) * 60 + action->getSecond(), timeNow = (hour * 60 + minute) * 60 + second;
  DaysMask dMask = action->getDaysMask();
  
  // Loop through 8 days (for actions scheduled today)
  for ( int i = today; i <= today + 7; i++ ) {
    // check if operation is scheduled for day i
    if ( dMask & (1 << (i % 7)) ) {
      // This is the first next day it will occur
      
      // If it is not today, or it happens after the timeNow, remember this day
      if (i != today || timeNow < timeLeft) {
        nextday = i;
        break;
      }
    }
  }

  // return the number of seconds left
  return ((timeLeft - timeNow) + (nextday - today) * 24 * 60 * 60);
}

time_t getSecondsUntil(Action *action) {
  // return the number of seconds left from now
  return getSecondsFrom(action, weekday(), hour(), minute(), second());
}

time_t getSecondsBetween(Action *actionStart, Action *action) {
  // find the first day of execution of actionStart after today
  DaysMask dMask = actionStart->getDaysMask();
  uint8_t nextDay;
  for ( int i = weekday(); i < weekday() + 7; i++) {
    if (dMask & (1 << (i%7))) {
      nextDay = (i%7);
      break;
    }
  }

  // return the number of seconds between actionStart and action
  return getSecondsFrom(action,nextDay, actionStart->getHour(), actionStart->getMinute(), actionStart->getSecond());
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
    debug("Executed in "+String(getSecondsUntil(nextAction))+" seconds");
    
    nextAction = nextAction->getNextAction();
  } while (nextAction != NULL);
}
