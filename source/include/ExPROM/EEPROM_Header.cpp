#include "EEPROM_Header.h"

void EEPROM_Header::reset() {
  // Reset the indexes
  this->resetActionIndex();
  this->resetMeasurementIndex();
}

void EEPROM_Header::resetActionIndex() {
  this->setActionIndex(0);
}

void EEPROM_Header::resetMeasurementIndex() {
  this->setMeasurementIndex(0);
}

uint16_t EEPROM_Header::getActionIndex() const {
  return (this->actionIndex);
}

uint16_t EEPROM_Header::getMeasurementIndex() const {
  return (this->measurementIndex);
}

void EEPROM_Header::setActionIndex(uint16_t newIndex) {
  this->actionIndex = newIndex;
}

void EEPROM_Header::setMeasurementIndex(uint16_t newIndex) {
  this->measurementIndex = newIndex;
}

uint16_t EEPROM_Header::increaseActionIndex(uint8_t increase) {
  return (this->actionIndex += increase);
}

uint16_t EEPROM_Header::increaseMeasurementIndex() {
  this->measurementIndex++;
}
