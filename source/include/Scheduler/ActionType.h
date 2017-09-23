#ifndef SCHEDULER_ACTIONTYPE_H_
#define SCHEDULER_ACTIONTYPE_H_

#include "Arduino.h"

// Enum for the action types
// Limited to 16 action types!
enum ActionTypeEnum : uint8_t {
  EMPTY_ACTION,
  AC_START,
  AC_STOP,
  UPDATE_CYCLE,
  UPDATE_SCHEDULE,
  SEND_MEASURE
};

class ActionType {
  private:
  ActionTypeEnum aType;

  public:
  // Constructors
  ActionType(ActionTypeEnum actionType);

  // Getters
  uint8_t getRawValue() const;

  // Elaborate getters
  String print() const;

  // Setters
  void setActionType(ActionTypeEnum actionType);
};
#endif
