#ifndef CYCLEMANAGER_LIB_H_
#define CYCLEMANAGER_LIB_H_

#include "Arduino.h"

// Constants for the cycles
// Cycle time in s
#define CYCLE_TIME 30
#define CYCLE_FACTOR 1.02

// Minimum time in s between each adjust cycle factor
#define CYCLE_ADJUST_FACTOR_MIN_TIME  60

void initializeCycleManager();

void resetCycleManager();

void updateCycleManager();

uint32_t getNextCycle();

void updateCycleFactor(int32_t timeShift, uint32_t timeSpan);

void updateCycleTime(uint32_t newCycleTime);

void updateTime(String timestamp); 

uint16_t getCycleTime();

uint16_t getIterationsFromLastCycle();

time_t getCurrentCycleStart();
#endif
