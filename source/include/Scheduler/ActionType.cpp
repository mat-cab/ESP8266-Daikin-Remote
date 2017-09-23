#include "ActionType.h"

ActionType::ActionType(ActionTypeEnum actionType) {
  this->aType = actionType;
}

uint8_t ActionType::getRawValue() const {
  return (this->aType);
}

String ActionType::print() const {
  String result;

  switch (this->aType) {
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

void ActionType::setActionType(ActionTypeEnum actionType) {
  this->aType = actionType;
}

