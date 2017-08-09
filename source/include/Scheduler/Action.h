#ifndef SCHEDULER_ACTION_H_
#define SCHEDULER_ACTION_H_

#include "Arduino.h"

#include "TimeMask.h"
#include "ActionType.h"

class Action {
  // Attributes
  private:
  
  ActionType aType;
  TimeMask *tMask;
  Action *nextAction;

  // Public methods
  public:
  // Constructors
  Action();
  Action( ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);

  DaysMask getDaysMask() const;

  uint8_t getHour() const;

  uint8_t getMinute() const;

  uint8_t getSecond() const;

  String getActionType() const;

  Action * getNextAction() const;

  Action * addAction(Action * newAction);
  Action * addAction(Action newAction);
};

#endif
