#include "Arduino.h"

#include "Time.h"

#include "../RTCMem_Lib.h"
#include "../Debug_Lib.h"

#include "Action.h"

Action::Action() {
  this->tMask = new TimeMask();
  this->aMask = new ActionMask();

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

Action::Action(TimeMask * timeMask, ActionMask * aMask) : Action() {
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
  //TODO: Complete the run function for each case

  // for debug purposes
  debug("Running following action:");
  this->print();
  
  // mark as executed for today
  this->aMask->setExecuted(true);
}

void Action::print() const {
  debug(printActionType(getActionType())+" - happens on "+String(this->tMask->getDaysMask())+" at "+String(this->tMask->getHour())+":"+String(this->tMask->getMinute())+":"+String(this->tMask->getSecond()));
}

Action * parseActionFromString(String actionString) {
  debug("Parsing action : "+actionString);

  byte argumentNumber = 0;
  char * argument = strtok(const_cast<char*>(actionString.c_str()),SCHEDULER_ACTION_PARSER_DELIMITER);
  ActionType aType;
  byte daysMask = 0, hours = 0, minutes = 0, seconds = 0;
  Action * result = NULL;

  while (argument != NULL) {
    
    switch(argumentNumber) {
      case 0:
        // This is the action type
        aType = getActionType( argument );
        break;
      case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        // This is the days flag
        if ( strcmp( argument, "1" ) == 0) {
          // Inputs are Mon;Tues;Wed;Thu;Fri;Sat;Sun
          // Whereas daysMask is Sat;Sun;Mon;Tues;Wed;Thu;Fri;
          daysMask += (1 << ((argumentNumber + 1) % 7));
        }
        break;
      case 8:
        // This is for the hours
        hours = (byte)atoi(argument);
        break;
      case 9:
        // This is for the minutes
        minutes = (byte)atoi(argument);
        break;
      case 10:
        // This is for the seconds
        seconds = (byte)atoi(argument);
        break;
    }

    // Loop for the next argument
    argumentNumber++;
    argument = strtok(NULL,SCHEDULER_ACTION_PARSER_DELIMITER);
  }
  
  // Say there was an error for unknown actions
  if (aType == UNKNOWN_ACTION) {
    debug("Error while parsing this action!");
  } else {
    result = new Action(aType, daysMask, hours, minutes, seconds);
  }

  return result;
}
