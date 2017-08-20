#include "Arduino.h"

#include "Time.h"

#include "../RTCMem_Lib.h"
#include "../Debug_Lib.h"

#include "Action.h"

Action::Action() {
  aType = EMPTY_ACTION;

  TimeMask * timeMask = new TimeMask();
  tMask = timeMask;

  nextAction = NULL;

  executed = false;
}

Action::Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action() {
  aType = actionType;

  tMask->setDaysMask(&dMask);
  tMask->setHour(hour);
  tMask->setMinute(minute);
  tMask->setSecond(second);
}

Action::Action(TimeMask * timeMask) : Action() {
  tMask = timeMask;
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

bool Action::isExecuted() const {
  return executed;
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

void Action::print() const {
  debug(this->getActionType()+" - happens on "+String(this->getDaysMask())+" at "+String(this->getHour())+":"+String(this->getMinute())+":"+String(this->getSecond()));
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

void Action::makeLastAction() {
  nextAction = NULL;
}

void Action::run() {
  //TODO: Complete the run function for each case

  // for debug purposes
  debug("Running following action:");
  this->print();
  
  // mark as executed
  this->executed = true;
}

void Action::saveInRTCMem() const {
  TimeMask * RTCTM = getRTCPointer_nextActionTimeMask();

  *RTCTM = *tMask;
}

Action * Action::readFromRTCMem() {
  TimeMask * RTCTM = getRTCPointer_nextActionTimeMask();

  return new Action( RTCTM );
}
