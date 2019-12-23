#include "AC_FanSpeed.h"

String printACFanSpeed( AC_FANSPEED fanSpeed ) {
  switch (fanSpeed) {
    case LOW_FANSPEED:
      return "LOW";
    case MEDIUM_FANSPEED:
      return "MEDIUM";
    case HIGH_FANSPEED:
      return "HIGH";
    default:
      return "AUTO";
  }
}

AC_FANSPEED getACFanSpeedFromString( String acFanSpeed ) {
  if (acFanSpeed.equals("LOW")) {
    return LOW_FANSPEED;
  } else if (acFanSpeed.equals("MEDIUM")) {
    return MEDIUM_FANSPEED;
  } else if (acFanSpeed.equals("HIGH")) {
    return HIGH_FANSPEED;
  } else {
    return AUTO_FANSPEED;
  }
}
