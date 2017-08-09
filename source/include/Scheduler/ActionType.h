#ifndef SCHEDULER_ACTIONTYPE_H_
#define SCHEDULER_ACTIONTYPE_H_

#include "Arduino.h"

// Enum for the action types
enum ActionType : uint8_t {
  EMPTY_ACTION,
  AC_START,
  AC_STOP,
  UPDATE_CYCLE,
  UPDATE_SCHEDULE,
  SEND_MEASURE
};

#endif
