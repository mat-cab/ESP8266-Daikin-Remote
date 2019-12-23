#ifndef SCHEDULER_ACTIONS_AC_START_MODE_H_
#define SCHEDULER_ACTIONS_AC_START_MODE_H_

#include "Arduino.h"

// Enum for the action types
// Limited to 8 action types (cf Additional_Data_AC_Start.h)
enum AC_MODE : uint8_t {
  AUTO = 0,
  DRY,
  COOL,
  HEAT,
  FAN
};

String printACMode( AC_MODE mode );

AC_MODE getACModeFromString( String acMode ); 
#endif
