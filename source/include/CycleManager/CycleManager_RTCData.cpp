#include "CycleManager.h"

#include "CycleManager_RTCData.h"

CycleManager_RTCData::CycleManager_RTCData() {
  this->cycle_factor = CYCLE_FACTOR;
  this->cycle_time = CYCLE_TIME;
  this->timestamp = 0;
  this->iteration = 0;
}

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
