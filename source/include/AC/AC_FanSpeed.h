#ifndef AC_FANSPEED_H_
#define AC_FANSPEED_H_

#include "Arduino.h"

// Enum for the allowed fan speed
enum AC_FANSPEED : uint8_t {
  LOW_FANSPEED = 0,
  MEDIUM_FANSPEED,
  HIGH_FANSPEED,
  AUTO_FANSPEED
};

String printACFanSpeed( AC_FANSPEED fanSpeed );

AC_FANSPEED getACFanSpeedFromString( String acFanSpeed ); 
#endif
