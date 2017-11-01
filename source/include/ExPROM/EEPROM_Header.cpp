#include "EEPROM_Header.h"

EEPROM_Header::EEPROM_Header() {
  this->reset();
}

void EEPROM_Header::reset() {
  // Reset the indexes
  this->resetActionIndex();
  this->resetMeasurementIndex();
}

void EEPROM_Header::resetActionIndex() {
  this->setActionIndex(0);
}

void EEPROM_Header::resetMeasurementIndex() {
  this->measurementIndexStart = 0;
  this->measurementIndexEnd = 0;
}

void EEPROM_Header::resetMeasurementIndexEnd() {
  this->measurementIndexEnd = 0;
}

uint16_t EEPROM_Header::getActionIndex() const {
  return (this->actionIndex);
}

uint16_t EEPROM_Header::getMeasurementIndexStart() const {
  return (this->measurementIndexStart);
}

uint16_t EEPROM_Header::getMeasurementIndexEnd() const {
  return (this->measurementIndexEnd);
}

void EEPROM_Header::setActionIndex(uint16_t newIndex) {
  this->actionIndex = newIndex;
}

void EEPROM_Header::resetMeasurementIndexStart(uint16_t newStart) {
  this->measurementIndexStart = newStart;
}

uint16_t EEPROM_Header::increaseActionIndex(uint8_t increase) {
  return (this->actionIndex += increase);
}

uint16_t EEPROM_Header::increaseMeasurementIndexEnd() {
  return (this->measurementIndexEnd += 1);
}

uint16_t EEPROM_Header::increaseMeasurementIndexStart(uint16_t increase) {
  return (this->measurementIndexStart += increase);
}
