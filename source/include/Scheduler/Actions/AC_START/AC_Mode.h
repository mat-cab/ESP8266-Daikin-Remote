#ifndef SCHEDULER_ACTIONS_AC_START_MODE_H_
#define SCHEDULER_ACTIONS_AC_START_MODE_H_

#include "Arduino.h"

// Enum for the action types
// Limited to 16 action types!
enum AC_MODE : uint8_t {
  AUTO = 0,
  DRY = 2,
  COOL = 3,
  HEAT = 4,
  FAN = 5
};

String printACMode( AC_MODE mode );

AC_MODE getACModeFromString( String acMode ); 
#endif
