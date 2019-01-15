#include "CycleManager.h"

#include "CycleManager_RTCData.h"

float * CycleManager_RTCData::getCycleFactor() {
  return &(this->cycle_factor);
}

time_t * CycleManager_RTCData::getTimestamp() {
  return &(this->timestamp);
}

time_t * CycleManager_RTCData::getTimeElapsed() {
  return &(this->timeElapsed);
}

uint64_t * CycleManager_RTCData::getCycleTime() {
  return &(this->cycle_time);
}

uint64_t * CycleManager_RTCData::getRemainingSleepTime() {
  return &(this->remainingSleepTime);
}

uint32_t * CycleManager_RTCData::getTimeSlept() {
  return &(this->timeSlept);
}
