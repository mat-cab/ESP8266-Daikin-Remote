#include "AdditionalData_AC_START.h"

AdditionalData_AC_START::AdditionalData_AC_START() {
  this->rawData = 0;
}

AdditionalData_AC_START::AdditionalData_AC_START(uint8_t * dataPointer) {
  this->rawData = *((uint16_t*)dataPointer);
}

uint8_t * AdditionalData_AC_START::getRawData() {
  return ((uint8_t *)&(this->rawData));
}
