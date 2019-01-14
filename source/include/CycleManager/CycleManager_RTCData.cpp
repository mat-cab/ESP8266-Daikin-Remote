#include "CycleManager.h"

#include "CycleManager_RTCData.h"

float * CycleManager_RTCData::getCycleFactor() {
  return &(this->cycle_factor);
}

time_t * CycleManager_RTCData::getTimestamp() {
  return &(this->timestamp);
}

time_t * CycleManager_RTCData::getLastUpdateTimestamp() {
  return &(this->lastUpdateTimestamp);
}

uint16_t * CycleManager_RTCData::getIteration() {
  return &(this->iteration);
}

uint64_t * CycleManager_RTCData::getCycleTime() {
  return &(this->cycle_time);
}

uint16_t * CycleManager_RTCData::getLastIteration() {
  return &(this->lastIteration);
}

uint64_t * CycleManager_RTCData::getRemainingSleepTime() {
  return &(this->remainingSleepTime);
}
