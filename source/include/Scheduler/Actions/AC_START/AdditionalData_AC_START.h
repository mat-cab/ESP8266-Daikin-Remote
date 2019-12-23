#ifndef SCHEDULER_ADDITIONAL_DATA_AC_START_H_
#define SCHEDULER_ADDITIONAL_DATA_AC_START_H_

#include "Arduino.h"

#include "../../AbstractAdditionalData.h"

#include "../../../AC/AC_Mode.h"
#include "../../../AC/AC_FanSpeed.h"

class AdditionalData_AC_START : public AbstractAdditionalData {
  private:
  // 2 bytes are necessary for the AC_START action:
  // Bits 0 to 2: AC_MODE
  // Bits 3 to 6: Temperature (start at 18 deg C)
  // Bits 7 to 9: AC_FANSPEED
  // Bits 10 to 15: boolean options (depends on IR protocol)
  uint16_t rawData;

  public:
  // Constructors
  AdditionalData_AC_START();
  AdditionalData_AC_START(uint8_t * dataPointer);

  // Getters
  uint8_t * getRawData();  
 
  // Elaborate getters
  AC_MODE getACMode() const;
  uint8_t getTemperature() const;
  AC_FANSPEED getFanSpeed() const;
  bool getBooleanOption(uint8_t optionIndex) const;

  // Elaborate setters
  void setACMode(String mode);
  void setTemperature(uint8_t temperature);
  void setFanSpeed(String fanSpeed);
  void setBooleanOption(uint8_t optionIndex, bool optionValue);

  // Debug functions
  void print() const;
};

#endif
