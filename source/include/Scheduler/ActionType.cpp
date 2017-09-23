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
    default:
      result = "UNKNOWN_ACTION";
      break;
  }

  return result;
}
