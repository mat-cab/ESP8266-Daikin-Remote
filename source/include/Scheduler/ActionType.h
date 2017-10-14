#ifndef SCHEDULER_ACTIONTYPE_H_
#define SCHEDULER_ACTIONTYPE_H_

#include "Arduino.h"

// Enum for the action types
// Limited to 16 action types!
enum ActionType : uint8_t {
  UNKNOWN_ACTION,
  EMPTY_ACTION,
  AC_START,
  AC_STOP,
  UPDATE_CYCLE,
  UPDATE_SCHEDULE,
  SEND_MEASURE
};

String printActionType( ActionType aType );

ActionType getActionType( String aTypeString ); 
#endif
