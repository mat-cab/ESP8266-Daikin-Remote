#ifndef CYCLEMANAGER_LIB_H_
#define CYCLEMANAGER_LIB_H_

#include "Arduino.h"

// Constants for the cycles
// Cycle time in ms
#define CYCLE_TIME 30
#define CYCLE_FACTOR 1.02

void initializeCycleManager();

void resetCycleManager();

void updateCycleManager();

uint32_t getNextCycle();

void updateCycleFactor(uint32_t timeShift, uint32_t timeSpan);

void updateTime(String timestamp); 

uint16_t getCycleTime();
#endif
