#include "Arduino.h"

#include "Time.h"

#include "../RTCMem_Lib.h"
#include "../Debug_Lib.h"

#include "ActionStaticFunctions.h"

#include "Action.h"

Action::Action() {
  this->tMask = new TimeMask();
  this->aMask = new ActionMask();
  this->aData = NULL;

  this->nextAction = NULL;
}

Action::Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action() {
  this->aMask->setActionType(actionType);

  this->tMask->setDaysMask(&dMask);
  this->tMask->setHour(hour);
  this->tMask->setMinute(minute);
  this->tMask->setSecond(second);

  setExecutionFlag();
}

Action::Action(TimeMask * timeMask, ActionMask * aMask, uint8_t * aDataPointer) : Action() {
  this->tMask = timeMask;
  this->aMask = aMask;
}

void Action::setExecutionFlag() {
  // Set the execution mask wrt seconds until next execution
  this->aMask->setExecuted((getSecondsFromNow() < 0));
}

Action * Action::getNextAction() const {
  return (this->nextAction);
}

TimeMask * Action::getTimeMask() const {
  return (this->tMask);
}

ActionMask * Action::getActionMask() const {
  return (this->aMask);
}

AbstractAdditionalData * Action::getAdditionalData() const {
  return (this->aData);
}

ActionType Action::getActionType() const {
  return (this->aMask->getActionType());
}

bool Action::isLastAction() const {
  return (this->nextAction == NULL);
}

int32_t Action::getSecondsFromNow() const {
  uint8_t today = weekday(), nextday;
  int32_t timeLeft = (this->tMask->getHour() * 60 + this->tMask->getMinute()) * 60 + this->tMask->getSecond(), timeNow = (hour() * 60 + minute()) * 60 + second();
 
  DaysMask dMask = this->tMask->getDaysMask();
  
  // Loop through 8 days (for actions scheduled today)
  for ( uint8_t i = today; i <= today + 7; i++ ) {
    // check if operation is scheduled for day i
    if ( dMask & (1 << (i % 7)) ) {
      // This is the first next day it will occur
      
      // If it is not today, then this is the next day
      // If it is today, check if it was already executed (otherwise it is for next week)
      if (i != today || !this->aMask->isExecuted()) {
        nextday = i;
        break;
      }
    }
  }

  // return the number of seconds left
  return ((timeLeft - timeNow) + (nextday - today) * 24 * 60 * 60);
}

void Action::resetExecuted() {
  this->aMask->setExecuted(false);
}

void Action::resetAllExecuted() {
  resetExecuted();

  if (this->nextAction != NULL) {
    return this->nextAction->resetAllExecuted();
  }
}

Action * Action::addAction( Action * newAction ) {
  // First of all, check if there is an action after
  if (this->nextAction == NULL) {
    // If not, then put it at the end
    this->nextAction = newAction;
    // Also say newAction is the last one
    newAction->nextAction = NULL;

    // do not do anything else
    return NULL;
  }

  // If there is a next action, verify if the new action is before or after this next one
  if (newAction->getSecondsFromNow() > this->nextAction->getSecondsFromNow()) {
    // newAction is after the next one
    this->nextAction->addAction( newAction );
  } else {
    // newAction is after the current one
    newAction->nextAction = this->nextAction;
    this->nextAction = newAction;
  }
}

Action * Action::addAction(Action newAction) {
  return addAction(&newAction);
}

void Action::run() {
  // for debug purposes
  debug("Running following action:");
  this->print();
  
  // run the appropriate action
  this->runAction();

  // mark as executed for today
  this->aMask->setExecuted(true);
}

// Virtual methods (empty in Action)
void Action::initializeAdditionalData(uint8_t * aDataPointer) { }
void Action::runAction() const { }
void Action::addAdditionalActionData(char * argument, uint8_t argumentNumber) const { }

void Action::print() const {
  debug(printActionType(getActionType())+" - happens on "+String(this->tMask->getDaysMask())+" at "+String(this->tMask->getHour())+":"+String(this->tMask->getMinute())+":"+String(this->tMask->getSecond())+" - secondary data: "+String(this->aMask->getSecondaryData()));

  if (this->aData != NULL) {
    this->aData->print();
  }
}
