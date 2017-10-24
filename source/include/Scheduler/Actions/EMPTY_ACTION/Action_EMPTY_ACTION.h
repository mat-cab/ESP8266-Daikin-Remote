#ifndef SCHEDULER_ACTIONS_EMPTY_ACTION_H_
#define SCHEDULER_ACTIONS_EMPTY_ACTION_H_

#include "../../Action.h"

class Action_EMPTY_ACTION : public Action {
  public:
  // Constructors
  Action_EMPTY_ACTION(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action_EMPTY_ACTION(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // initializer
  void runAction() const;  

  void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;
};

#endif
