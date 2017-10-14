#ifndef SCHEDULER_ADDITIONAL_DATA_UPDATE_CYCLE_H_
#define SCHEDULER_ADDITIONAL_DATA_UPDATE_CYCLE_H_

#include "Arduino.h"

#include "AbstractAdditionalData.h"

class AdditionalData_UPDATE_CYCLE : public AbstractAdditionalData {
  private:
  // 3 bytes are necessary for the UPDATE_CYCLE action
  // Bits 0 to 23 : new cycle in ms (max 16777215 ms, approx 279 minutes)
  uint8_t rawData[3]; 

  public:
  // Constructors
  AdditionalData_UPDATE_CYCLE();
  AdditionalData_UPDATE_CYCLE(uint8_t * dataPointer);

  // Getters
  uint8_t * getRawData();

  // Elaborate getters
  uint32_t getCycleTime() const;

  // Setters

  // Elaborate setters
  void setCycleTime(uint32_t newCycle);
};
#endif
