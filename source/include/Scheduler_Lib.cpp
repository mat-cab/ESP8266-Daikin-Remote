#include "Time.h"

#include "Debug_Lib.h"

#include "Scheduler_Lib.h"

Action * schedule;

void initializeScheduler() {
  // Fill with stuff initially
  Action * newAction = new Action(AC_START, 0x01, 17, 30, 0);
  schedule = newAction;

  newAction = new Action(Action(AC_STOP, 0x7F, 15, 30, 0));
  schedule->addAction(newAction);

  newAction = new Action(Action(AC_STOP, 0x7F, 23, 59, 59));
  schedule->addAction(newAction);

  newAction = new Action(Action(AC_STOP, 0x08, 20, 0, 0));
  schedule->addAction(newAction);
}

void clearScheduler() {
  schedule = NULL;
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

  while ( currentAction->getNextAction() != NULL ) {
    currentAction = nextAction->getNextAction();

    nextTime = getSecondsUntil(nextAction);

    if (nextTime < minTime) {
      minTime = nextTime;
      nextAction = currentAction;
    }
  }

  return nextAction;
}

time_t getSecondsUntil(Action *action) {
  uint8_t today = weekday(), nextday;
  time_t timeLeft = (action->getHour() * 60 + action->getMinute()) * 60 + action->getSecond(), timeNow = (hour() * 60 + minute()) * 60 + second();
  DaysMask dMask = action->getDaysMask();
  
  // Loop through 8 days (for actions scheduled today)
  for ( int i = today; i <= today + 7; i++ ) {
    // check if operation is scheduled for day i
    if ( dMask & (1 << (i % 7)) ) {
      // This is the first next day it will occur
      
      // If it is today, and that the time is before now, then it is not for today
      if (i == today && timeNow > timeLeft) {
        continue;
      }

      // Otherwise remember the day
      nextday = i;
      // And stop searching
      break;
    }
  }

  // return the number of seconds left
  return ((timeLeft - timeNow) + (nextday - today) * 24 * 60 * 60);
} 

void printSchedule() {
  uint8_t i = 1;
  Action *nextAction = schedule;

  do {
    debug("Schedule place "+String(i++)+":");
    printAction(nextAction);
    
    nextAction = nextAction->getNextAction();
  } while (nextAction != NULL);
}

void printAction(Action *action) {
  String aTypeString = action->getActionType();

  debug(aTypeString+" - happens on "+String(action->getDaysMask())+" at "+String(action->getHour())+":"+String(action->getMinute())+":"+String(action->getSecond()));
}
