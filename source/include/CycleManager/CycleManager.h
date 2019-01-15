#ifndef CYCLEMANAGER_LIB_H_
#define CYCLEMANAGER_LIB_H_

#include "Arduino.h"

// Constants for the cycles
// Cycle time in s
#define CYCLE_TIME 600
#define CYCLE_FACTOR 1.00

// Minimum time in s between each adjust cycle factor
#define CYCLE_ADJUST_FACTOR_MIN_TIME  60

// Maximum time for ta single deepsleep in us
#define CYCLE_MAX_DEEPSLEEP_TIME 60E6

void initializeCycleManager(bool corruptedMemory);

void resetCycleManager();

void updateCycleManager();

void updateDeepSleepTimers(uint64_t sleepTime);

void goToDeepSleep();

uint64_t getNextCycle();

void updateCycleFactor(int32_t timeShift, uint32_t timeSpan);

void updateCycleTime(uint32_t newCycleTime);

int32_t updateTime(String timestamp); 

uint16_t getCycleTime();

uint16_t getIterationsFromLastCycle();

time_t getCurrentCycleStart();
#endif
