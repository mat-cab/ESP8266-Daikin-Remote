#ifndef SCHEDULER_ADDITIONAL_DATA_AC_START_H_
#define SCHEDULER_ADDITIONAL_DATA_AC_START_H_

#include "Arduino.h"

#include "../../AbstractAdditionalData.h"

#include "AC_Mode.h"

class AdditionalData_AC_START : public AbstractAdditionalData {
  private:
  // 2 bytes are necessary for the AC_START action:
  // Bits 0 to 2: AC_MODE
  // Bits 3 to 6: Temperature (start at 18 deg C)
  // Bits 7 to 10: Fan speed
  // Bit 11: Enable swing control left/right
  // Bit 12: Enable swing control up/down
  // Bit 13: Enable powerful
  // Bit 14: Enable silent
  // Bit 15: Enable intelligent eye
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
  uint8_t getFanSpeed() const;
  bool getSwingLR() const;
  bool getSwingUD() const;
  bool getPowerful() const;
  bool getSilent() const;
  bool getIntelligentEye() const;

  // Elaborate setters
  void setACMode(String mode);
  void setTemperature(uint8_t temperature);
  void setFanSpeed(uint8_t fanSpeed);
  void setSwingLR(bool enable);
  void setSwingUD(bool enable);
  void setPowerful(bool enable);
  void setSilent(bool enable);
  void setIntelligentEye(bool enable);

  // Debug functions
  void print() const;
};

#endif
