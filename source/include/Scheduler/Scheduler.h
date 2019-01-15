/*
  Scheduler library

  Used to schedule the different actions
*/
#ifndef SCHEDULER_LIB_H_
#define SCHEDULER_LIB_H_

// Sleep time in case the schedule server is not available in ms
#define SCHEDULER_RESET_ERROR_SLEEP 3600E3

// Option to set the scheduler as strict
// If defined, then the actions will be performed approximately on time, not necessarily on a cyclemanager iteration
#define SCHEDULER_STRICT

#include "Arduino.h"

#include "Scheduler/Action.h"

void initializeScheduler();

bool resetScheduler(bool hardReset = false);

void addAction(ActionType actionType, DaysMask dMask, byte hour, byte minute, byte second);

void clearScheduler();

void runScheduler();

void saveSchedulerInRTCMem();

void sortSchedule();

bool isSchedulerActive();

uint64_t remainingTimeBeforeNextAction();

void printSchedule();

void printAction(Action *action);

void parseNewAction(String actionString);
#endif
