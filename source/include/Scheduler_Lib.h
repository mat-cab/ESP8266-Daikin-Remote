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

void updateScheduler();

void updateScheduler(Action *firstAction);

bool isSchedulerActive();

Action *getNextAction();

Action *getNextActionAfter(Action *actionStart);

time_t getSecondsFrom(Action *action, uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second);

time_t getSecondsUntil(Action *action);

time_t getSecondsBetween(Action *actionStart, Action *action);

void printSchedule();

void printAction(Action *action);

String getActionTypeString(ActionType aType);
#endif
