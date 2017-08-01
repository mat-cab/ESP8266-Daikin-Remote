#include "Arduino.h"

#include "Time.h"
#include "Debug_Lib.h"

#include "Scheduler_Lib.h"

Action schedule[15];
byte lastAction;

void initializeScheduler() {
  lastAction = 0;

  // Fill with stuff initially
  addAction(Stop, 0x01, 17, 30, 0);
  addAction(Start, 0x7F, 15, 30, 0);
}

void addAction(ActionType actionType, DaysMask dMask, byte hour, byte minute, byte second) {
  schedule[lastAction++] = Action { actionType, dMask, hour, minute, second };
}

void clearScheduler() {
  lastAction = 0;
}

bool isSchedulerActive() {
  return (lastAction > 0);  
}

Action *getNextAction() {
  // If the schedule is empty, return 0
  if (lastAction == 0) {
    return NULL;
  }

  // initialize the next action to the first one
  Action *nextAction = &(schedule[0]);
  time_t minTime = getSecondsUntil(nextAction), nextTime;

  for (int i = 1; i < lastAction; i++) {
    nextTime = getSecondsUntil(&(schedule[lastAction]));

    if (nextTime < minTime) {
      minTime = nextTime;
      nextAction = &(schedule[lastAction]);
    }
  }

  return nextAction;
}

time_t getSecondsUntil(Action *action) {
  uint8_t today = weekday(), nextday;
  time_t timeLeft = (action-> hour * 60 + action->minute) * 60 + action->second, timeNow = (hour() * 60 + minute()) * 60 + second();
  
  // Loop through 8 days (for actions scheduled today)
  for ( int i = today; i <= today + 7; i++ ) {
    // check if operation is scheduled for day i
    if ( action->dMask & (2 << (i % 7)) ) {
      // This is the first next day it will occur
      
      // If it is today, and that the time is before now, then it is not for today
      if (i == today && timeNow < timeLeft) {
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
  for (int i = 0; i < lastAction; i++) {
    debug("Schedule place "+String(i)+":");
    printAction(&(schedule[i]));
  }
}

void printAction(Action *action) {
  String aTypeString = getActionTypeString(action->aType);

  debug(aTypeString+" - happens on "+String(action->dMask)+" at "+String(action->hour)+":"+String(action->minute)+":"+String(action->second));
}

String getActionTypeString(ActionType aType) {
  String result;

  switch (aType) {
    case Start:
      result = "START";
      break;
    case Stop:
      result = "STOP";
      break;
    default:
      debug("Unknown ActionType :"+aType);
      break;
  }

  return result;
}
