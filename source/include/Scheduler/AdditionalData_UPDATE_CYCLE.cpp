#include "AdditionalData_UPDATE_CYCLE.h"

AdditionalData_UPDATE_CYCLE::AdditionalData_UPDATE_CYCLE() {
  this->rawData[0] = 0;
  this->rawData[1] = 0;
  this->rawData[2] = 0;
}

AdditionalData_UPDATE_CYCLE::AdditionalData_UPDATE_CYCLE(uint8_t * dataPointer) {
  this->rawData[0] = *dataPointer;
  this->rawData[1] = *(dataPointer + 1);
  this->rawData[2] = *(dataPointer + 2);
}

uint8_t * AdditionalData_UPDATE_CYCLE::getRawData() {
  return ((uint8_t *)this->rawData);
}

uint32_t AdditionalData_UPDATE_CYCLE::getCycleTime() const {
  uint32_t result = 0;

  result = this->rawData[0] | this->rawData[1] << 7 | this->rawData[2] << 14;

  return result;
}

void AdditionalData_UPDATE_CYCLE::setCycleTime(uint32_t newCycle) {
  this->rawData[0] = (newCycle & 0xFF);
  this->rawData[1] = ((newCycle & 0xFF00) >> 7);
  this->rawData[2] = ((newCycle & 0xFF0000) >> 14);
}
