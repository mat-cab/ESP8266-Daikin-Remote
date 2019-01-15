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
  // 4 bytes : Time elapsed since the start of cycle
  time_t timeElapsed;
  // 2 bytes: Cycle time
  uint64_t cycle_time;
  // 8 bytes : Time remaining to sleep in machine cycles (in us + converted with cycleFactor)
  uint64_t remainingSleepTime;
  // 4 bytes : Time slept wrt last iteration in s
  uint32_t timeSlept;

  public:
  // Getters
  float * getCycleFactor();
  time_t * getTimestamp();
  time_t * getTimeElapsed();
  uint64_t * getCycleTime();
  uint64_t * getRemainingSleepTime();
  uint32_t * getTimeSlept();
};
#endif
