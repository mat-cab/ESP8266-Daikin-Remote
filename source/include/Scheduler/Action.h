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
  bool executed;

  // private constructor (for the timemask)
  Action(TimeMask * timeMask);

  // Public methods
  public:
  // Constructors
  Action();
  Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);

  DaysMask getDaysMask() const;

  uint8_t getHour() const;

  uint8_t getMinute() const;

  uint8_t getSecond() const;

  bool isExecuted() const;

  String getActionType() const;

  void print() const;

  Action * getNextAction() const;

  Action * addAction(Action * newAction);
  Action * addAction(Action newAction);

  void makeLastAction();

  void run();

  // Methods for the RTC memory
  void saveInRTCMem() const;

  static Action * readFromRTCMem();
};

#endif
