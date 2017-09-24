#ifndef ACTIONMASK_H_
#define ACTIONMASK_H_

#include "Arduino.h"

#include "ActionType.h"

class ActionMask {
  private:
  // 1 bytes for some additional info
  // Bit 0: action already executed today
  // Bit 1 to 3: Secondary data:
  //            In the RTC Mem: last day of execution of the scheduler
  //            In EEPROM: length (in bytes) for the arguments of the action
  // Bit 4 to 7: type of the next action
  uint8_t rawData;

  public:
  // Constructors
  ActionMask();
  ActionMask(bool executed, ActionType actionType);
  ActionMask(uint8_t * dataPointer);

  // Getters
  uint8_t * getRawData();

  // Elaborate getters
  bool isExecuted() const;
  uint8_t getSecondaryData() const;
  ActionType getActionType() const;
  bool isEmpty() const;

  // Setters
  void setActionType(ActionType actionType);
  void setExecuted(bool executed);
  void setSecondaryData(uint8_t data);
};
#endif
