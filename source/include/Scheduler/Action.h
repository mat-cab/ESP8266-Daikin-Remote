#ifndef SCHEDULER_ACTION_H_
#define SCHEDULER_ACTION_H_

#define SCHEDULER_ACTION_PARSER_DELIMITER ";"

#include "Arduino.h"

#include "TimeMask.h"
#include "ActionType.h"
#include "ActionMask.h"
#include "AbstractAdditionalData.h"

class Action {
  // Attributes
  protected:
  // Internal variable
  // For data storage
  TimeMask *tMask;
  ActionMask *aMask;
  AbstractAdditionalData *aData;

  Action *nextAction;

  // Elaborate setters
  void setExecutionFlag();

  // Public methods
  public:
  // Constructors
  Action();
  Action(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // Initializers

  // Getters
  Action * getNextAction() const;
  TimeMask * getTimeMask() const;
  ActionMask * getActionMask() const;
  ActionType getActionType() const;
  AbstractAdditionalData * getAdditionalData() const;

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

  // Virtual functions
  virtual void initializeAdditionalData(uint8_t * aDataPointer);
  virtual void runAction() const;
  virtual void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;

  // Debug actions
  void print() const;
};
#endif
