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
  return (18 + ((this->rawData & 0x1E00) >> 9));
}

AC_FANSPEED AdditionalData_AC_START::getFanSpeed() const {
  return (AC_FANSPEED)((this->rawData & 0x01E0) >> 5);
}

bool AdditionalData_AC_START::getSwingLR() const {
  return ((this->rawData & 0x0010)>> 4);
}

bool AdditionalData_AC_START::getSwingUD() const {
  return ((this->rawData & 0x0008)>> 3);
}

bool AdditionalData_AC_START::getPowerful() const {
  return ((this->rawData & 0x0004)>> 2);
}

bool AdditionalData_AC_START::getSilent() const {
  return ((this->rawData & 0x0002)>> 1);
}

bool AdditionalData_AC_START::getIntelligentEye() const {
  return (this->rawData & 0x0001);
}

void AdditionalData_AC_START::setACMode(String mode) {
  AC_MODE acMode = getACModeFromString( mode );
  this->rawData = (this->rawData & 0x1FFF) | ((acMode & 0x07) << 13);
}

void AdditionalData_AC_START::setTemperature(uint8_t temperature) {
  this->rawData = (this->rawData & 0xE7FF) | (((temperature - 18) & 0x0F) << 9);
}

void AdditionalData_AC_START::setFanSpeed(uint8_t fanSpeed) {
  this->rawData = (this->rawData & 0xFE1F) | ((fanSpeed & 0x0F) << 5);
}

void AdditionalData_AC_START::setSwingLR(bool enable) {
  uint8_t flag = enable ? 1 : 0;  
  this->rawData = (this->rawData & 0xFFEF) | ((flag & 0x01) << 4);
}

void AdditionalData_AC_START::setSwingUD(bool enable) {
  uint8_t flag = enable ? 1 : 0;  
  this->rawData = (this->rawData & 0xFFF7) | ((flag & 0x01) << 3);
}

void AdditionalData_AC_START::setPowerful(bool enable) {
  uint8_t flag = enable ? 1 : 0;  
  this->rawData = (this->rawData & 0xFFFB) | ((flag & 0x01) << 2);
}

void AdditionalData_AC_START::setSilent(bool enable) {
  uint8_t flag = enable ? 1 : 0;  
  this->rawData = (this->rawData & 0xFFFD) | ((flag & 0x01) << 1);
}

void AdditionalData_AC_START::setIntelligentEye(bool enable) {
  uint8_t flag = enable ? 1 : 0;
  this->rawData = (this->rawData & 0xFFFE) | ((flag & 0x01));
}

void AdditionalData_AC_START::print() const {
  debug("Additional data:");
  debug("- AC_Mode: "+String(this->getACMode()));
  debug("- Temperature: "+String(this->getTemperature()));
  debug("- Fan speed: "+String(this->getFanSpeed()));
  debug("- Swing Left/Right: "+String(this->getSwingLR()));
  debug("- Swing Up/Down: "+String(this->getSwingUD()));
  debug("- Powerful: "+String(this->getPowerful()));
  debug("- Silent: "+String(this->getSilent()));
  debug("- Intelligent Eye: "+String(this->getIntelligentEye()));
} 
