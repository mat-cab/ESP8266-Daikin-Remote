#ifndef CYCLEMANAGER_LIB_H_
#define CYCLEMANAGER_LIB_H_

// Constants for the cycles
// Cycle time in ms
#define CYCLE_TIME 30
#define CYCLE_ITERATIONS 60
#define CYCLE_FACTOR 1.02

void initializeCycleManager();

void resetCycleManager();

void updateCycleManager();

uint32_t getNextCycle();

void updateTime(String timestamp); 

time_t getMeasurementTime(struct measurement * measure);
#endif
