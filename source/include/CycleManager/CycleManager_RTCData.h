#ifndef CYCLEMANAGER_RTCDATA_H_
#define CYCLEMANAGER_RTCDATA_H_

#include "Arduino.h"

class CycleManager_RTCData {
  private:
  // RTC data storage structure for the CycleManager
  // 4 bytes: Cycle factor
  float cycle_factor;
  // 4 bytes: Timestamp of the start of cycle
  time_t timestamp; 
  // 2 bytes: Iteration in the cycle
  uint16_t iteration;
  // 2 bytes: Cycle time
  uint16_t cycle_time;

  public:
  // Constructors
  CycleManager_RTCData();

  // Getters
  float * getCycleFactor();
  time_t * getTimestamp();
  uint16_t * getIteration();
  uint16_t * getCycleTime();
};
#endif
