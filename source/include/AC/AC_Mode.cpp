#include "AC_Mode.h"

String printACMode( AC_MODE mode) {
  String result;

  switch (mode) {
    case HEAT:
      result = "HEAT";
      break;
    case DRY:
      result = "DRY";
      break;
    case FAN:
      result = "FAN";
      break;
    case COOL:
      result = "COOL";
      break;
    default:
      result = "AUTO";
      break;
  }

  return result;
}

AC_MODE getACModeFromString( String acMode ) {
  AC_MODE result;

  if ( acMode.equals("HEAT")) {
    result = HEAT;
  } else if (acMode.equals("DRY")) {
    result = DRY;
  } else if (acMode.equals("FAN")) {
    result = FAN;
  } else if (acMode.equals("COOL")) {
    result = COOL;
  } else {
    result = AUTO;
  }

  return result;
}
