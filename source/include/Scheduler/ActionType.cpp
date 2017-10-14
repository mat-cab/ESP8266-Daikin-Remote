#include "ActionType.h"

String printActionType( ActionType aType) {
  String result;

  switch (aType) {
    case EMPTY_ACTION:
      result = "EMPTY_ACTION";
      break;
    case AC_START:
      result = "AC_START";
      break;
    case AC_STOP:
      result = "AC_STOP";
      break;
    case UPDATE_CYCLE:
      result = "UPDATE_CYCLE";
      break;
    case UPDATE_SCHEDULE:
      result = "UPDATE_SCHEDULE";
      break;
    case SEND_MEASURE:
      result = "SEND_MEASURE";
      break;
    default:
      result = "UNKNOWN_ACTION";
      break;
  }

  return result;
}

ActionType getActionType( String aTypeString ) {
  ActionType result;

  if ( aTypeString.equals("EMPTY_ACTION")) {
    result = EMPTY_ACTION;
  } else if (aTypeString.equals("AC_START")) {
    result = AC_START;
  } else if (aTypeString.equals("AC_STOP")) {
    result = AC_STOP;
  } else if (aTypeString.equals("UPDATE_CYCLE")) {
    result = UPDATE_CYCLE;
  } else if (aTypeString.equals("UPDATE_SCHEDULE")) {
    result = UPDATE_SCHEDULE;
  } else if (aTypeString.equals("SEND_MEASURE")) {
    result = SEND_MEASURE;
  } else {
    result = UNKNOWN_ACTION;
  }

  return result;
}
