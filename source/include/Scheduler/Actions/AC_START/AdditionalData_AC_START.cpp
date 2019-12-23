#include "../../../Debug_Lib.h"

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

AC_MODE AdditionalData_AC_START::getACMode() const {
  return (AC_MODE)((this->rawData & 0xE000) >> 13);
}

uint8_t AdditionalData_AC_START::getTemperature() const {
  return getAbsoluteTemperature((this->rawData & 0x1E00) >> 9);
}

AC_FANSPEED AdditionalData_AC_START::getFanSpeed() const {
  return (AC_FANSPEED)((this->rawData & 0x01C0) >> 6);
}

bool AdditionalData_AC_START::getBooleanOption(uint8_t optionIndex) const {
  uint8_t shift = 6 - optionIndex;
  uint8_t mask = 0x0001 << shift;
  return ((this->rawData & mask) >> shift);
}

void AdditionalData_AC_START::setACMode(String mode) {
  AC_MODE acMode = getACModeFromString( mode );
  this->rawData = (this->rawData & 0x1FFF) | ((acMode & 0x07) << 13);
}

void AdditionalData_AC_START::setTemperature(uint8_t temperature) {
  this->rawData = (this->rawData & 0xE7FF) | ((getProtocolTemperature(temperature) & 0x0F) << 9);
}

void AdditionalData_AC_START::setFanSpeed(String fanSpeed) {
  uint8_t acFanSpeed = getACFanSpeedFromString( fanSpeed ); 
  this->rawData = (this->rawData & 0xFE3F) | ((acFanSpeed & 0x07) << 6);
}

void AdditionalData_AC_START::setBooleanOption(uint8_t optionIndex, bool optionValue) {
  uint8_t shift = 6 - optionIndex;
  uint8_t mask = 0x0001 << shift;
  uint8_t flag = (optionValue ? 0x0001 : 0x0000) << shift;
  this->rawData = (this->rawData & ~mask) | flag;
}

void AdditionalData_AC_START::print() const {
  debug("Additional data:");
  debug("- AC_Mode: "+String(printACMode(this->getACMode())));
  debug("- Temperature: "+String(this->getTemperature()));
  debug("- Fan speed: "+String(printACFanSpeed(this->getFanSpeed())));
  for ( int i = 0; i < 7; i++ )
    debug("- Boolean option "+String(i)+": "+String(this->getBooleanOption(i)));
} 
