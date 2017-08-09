/*
  Scheduler library

  Used to schedule the different actions
*/
#ifndef SCHEDULER_LIB_H_
#define SCHEDULER_LIB_H_

#include "Arduino.h"

#include "Scheduler/Action.h"

void initializeScheduler();

void addAction(ActionType actionType, DaysMask dMask, byte hour, byte minute, byte second);

void clearScheduler();

bool isSchedulerActive();

time_t getSecondsUntil(Action *action);

void printSchedule();

void printAction(Action *action);

String getActionTypeString(ActionType aType);
#endif
