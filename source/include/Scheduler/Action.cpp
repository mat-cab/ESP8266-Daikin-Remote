#include "Arduino.h"

#include "../Debug_Lib.h"

#include "Action.h"

Action::Action() {
  aType = EMPTY_ACTION;

  TimeMask * timeMask = new TimeMask();
  tMask = timeMask;

  nextAction = NULL;
}

Action::Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action() {
  aType = actionType;

  tMask->setDaysMask(&dMask);
  tMask->setHour(hour);
  tMask->setMinute(minute);
  tMask->setSecond(second);
}

DaysMask Action::getDaysMask() const {
  return tMask->getDaysMask();
}

uint8_t Action::getHour() const {
  return tMask->getHour();
}

uint8_t Action::getMinute() const {
  return tMask->getMinute();
}

uint8_t Action::getSecond() const {
  return tMask->getSecond();
}

String Action::getActionType() const {
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
      debug("Unknown ActionType :"+aType);
      break;
  }

  return result;
}

Action * Action::getNextAction() const {
  return nextAction;
}

Action * Action::addAction( Action * newAction ) {
  if (nextAction != NULL) {
    return nextAction->addAction(newAction);
  }

  nextAction = newAction;
}

Action * Action::addAction(Action newAction) {
  return addAction(&newAction);
}
