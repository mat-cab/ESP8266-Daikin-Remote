#ifndef EEPROM_HEADER_H_
#define EEPROM_HEADER_H

#include "Arduino.h"

class EEPROM_Header {
  protected:
  // 2 bytes for the number of actions in the schedule
  uint16_t actionIndex;
  // 2 bytes for the number of measurements available
  uint16_t measurementIndex;

  public:
  // Initializers
  void reset();
  void resetActionIndex();
  void resetMeasurementIndex();

  // Getters
  uint16_t getActionIndex() const;
  uint16_t getMeasurementIndex() const;

  // Setters
  void setActionIndex(uint16_t newIndex);
  void setMeasurementIndex(uint16_t newIndex);

  // Elaborate setters
  uint16_t increaseActionIndex(uint8_t increase);
  uint16_t increaseMeasurementIndex();
};

#endif
