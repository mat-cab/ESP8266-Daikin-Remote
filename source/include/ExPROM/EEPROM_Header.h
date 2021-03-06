#ifndef EEPROM_HEADER_H_
#define EEPROM_HEADER_H_

#include "Arduino.h"

class EEPROM_Header {
  protected:
  // 2 bytes for the number of actions in the schedule
  uint16_t actionIndex;
  // 2 bytes for the start of the measurement
  uint16_t measurementIndexStart;
  // 2 bytes for the end of the measurement
  uint16_t measurementIndexEnd;

  public:
  // Constructor
  EEPROM_Header();

  // Initializers
  void reset();
  void resetActionIndex();
  void resetMeasurementIndex();
  void resetMeasurementIndexEnd();

  // Getters
  uint16_t getActionIndex() const;
  uint16_t getMeasurementIndexStart() const;
  uint16_t getMeasurementIndexEnd() const;

  // Setters
  void setActionIndex(uint16_t newIndex);
  void resetMeasurementIndexStart(uint16_t newStart);

  // Elaborate setters
  uint16_t increaseActionIndex(uint8_t increase);
  uint16_t increaseMeasurementIndexEnd();
  uint16_t increaseMeasurementIndexStart(uint16_t increase);
};

#endif
