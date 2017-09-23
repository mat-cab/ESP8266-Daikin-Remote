#ifndef SCHEDULER_ACTION_H_
#define SCHEDULER_ACTION_H_

#include "Arduino.h"

#include "TimeMask.h"
#include "ActionType.h"

class Action {
  // Attributes
  private:
  // Internal variable
  ActionType *aType;
  TimeMask *tMask;
  Action *nextAction;
  bool executed;

  // Elaborate setters
  bool setExecutionFlag();

  // Public methods
  public:
  // Constructors
  Action();
  Action(ActionTypeEnum actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action(TimeMask *tMask, bool executionFlag, ActionTypeEnum actionType);

  // Getters
  Action * getNextAction() const;
  TimeMask * getTimeMask() const;
  ActionType * getActionType() const;

  // Elaborate getters
  bool isLastAction() const;
  int32_t getSecondsFromNow() const;

  // Elaborate setters
  void resetExecuted();

  // Group setters
  void resetAllExecuted();
  Action * addAction(Action * newAction);
  Action * addAction(Action newAction);

  // Actions
  void run();

  // Debug actions
  void print() const;
};

#endif
