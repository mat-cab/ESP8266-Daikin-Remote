/*
  Scheduler library

  Used to schedule the different actions
*/
#ifndef SCHEDULER_LIB_H_
#define SCHEDULER_LIB_H_

#include "Arduino.h"

#include "Scheduler/Action.h"

void initializeScheduler();

void resetScheduler();

void addAction(ActionType actionType, DaysMask dMask, byte hour, byte minute, byte second);

void clearScheduler();

void runScheduler();

void sortSchedule();

bool isSchedulerActive();

void printSchedule();

void printAction(Action *action);

String getActionTypeString(ActionType aType);
#endif
