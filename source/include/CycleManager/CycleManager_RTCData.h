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
  // 4 bytes: Timestamp of the last time update
  time_t lastUpdateTimestamp;
  // 2 bytes: Iteration in the cycle
  uint16_t iteration;
  // 2 bytes: Cycle time
  uint16_t cycle_time;
  // 2 bytes: Last iteration in the cycle
  uint16_t lastIteration;

  public:
  // Getters
  float * getCycleFactor();
  time_t * getTimestamp();
  time_t * getLastUpdateTimestamp();
  uint16_t * getIteration();
  uint16_t * getCycleTime();
  uint16_t * getLastIteration();
};
#endif
