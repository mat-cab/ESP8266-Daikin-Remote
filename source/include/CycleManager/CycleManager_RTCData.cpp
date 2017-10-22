#include "CycleManager.h"

#include "CycleManager_RTCData.h"

float * CycleManager_RTCData::getCycleFactor() {
  return &(this->cycle_factor);
}

time_t * CycleManager_RTCData::getTimestamp() {
  return &(this->timestamp);
}

uint16_t * CycleManager_RTCData::getIteration() {
  return &(this->iteration);
}

uint16_t * CycleManager_RTCData::getCycleTime() {
  return &(this->cycle_time);
}

uint16_t * CycleManager_RTCData::getLastIteration() {
  return &(this->lastIteration);
}
