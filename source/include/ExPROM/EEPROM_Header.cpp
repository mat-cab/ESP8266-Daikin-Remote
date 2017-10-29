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

uint16_t EEPROM_Header::getActionIndex() const {
  return (this->actionIndex);
}

uint16_t EEPROM_Header::getMeasurementIndexStart() const {
  return (this->measurementIndexStart);
}

uint16_t EEPROM_Header::getMeasurementIndex() const {
  // Nominal case, end is after start
  if (this->measurementIndexEnd >= this->measurementIndexStart) {
    return (this->measurementIndexEnd - this->measurementIndexStart);
  } else {
    // degraded case, end is before start (in case of wrap around)
    return (this->measurementIndexEnd + (0xFFFF - this->measurementIndexStart) + 1);
  }
}

void EEPROM_Header::setActionIndex(uint16_t newIndex) {
  this->actionIndex = newIndex;
}

uint16_t EEPROM_Header::increaseActionIndex(uint8_t increase) {
  return (this->actionIndex += increase);
}

uint16_t EEPROM_Header::increaseMeasurementIndex() {
  return (this->measurementIndexEnd++);
}

uint16_t EEPROM_Header::increaseMeasurementIndexStart(uint16_t increase) {
  return (this->measurementIndexStart += increase);
}
