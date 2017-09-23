#ifndef SCHEDULER_ACTION_H_
#define SCHEDULER_ACTION_H_

#include "Arduino.h"

#include "TimeMask.h"
#include "ActionType.h"
#include "ActionMask.h"

class Action {
  // Attributes
  private:
  // Internal variable
  // For data storage
  TimeMask *tMask;
  ActionMask *aMask;

  Action *nextAction;

  // Elaborate setters
  void setExecutionFlag();

  // Public methods
  public:
  // Constructors
  Action();
  Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action(TimeMask *tMask, ActionMask *aMask);

  // Getters
  Action * getNextAction() const;
  TimeMask * getTimeMask() const;
  ActionMask * getActionMask() const;
  ActionType getActionType() const;

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