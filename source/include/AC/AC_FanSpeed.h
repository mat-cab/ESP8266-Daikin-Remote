#ifndef AC_FANSPEED_H_
#define AC_FANSPEED_H_

#include "Arduino.h"

// Enum for the allowed fan speed
// limited to 8 fan speeds (cf Additional_Data_AC_Start.h)
enum AC_FANSPEED : uint8_t {
  AUTO_FANSPEED = 0,
  LOW_FANSPEED,
  MEDIUM_FANSPEED,
  HIGH_FANSPEED
};

String printACFanSpeed( AC_FANSPEED fanSpeed );

AC_FANSPEED getACFanSpeedFromString( String acFanSpeed ); 
#endif
