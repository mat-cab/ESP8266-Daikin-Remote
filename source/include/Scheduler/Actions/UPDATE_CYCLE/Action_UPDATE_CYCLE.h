#ifndef SCHEDULER_ACTIONS_UPDATE_CYCLE_H_
#define SCHEDULER_ACTIONS_UPDATE_CYCLE_H_

#include "../../Action.h"

class Action_UPDATE_CYCLE : public Action {
  public:
  // Constructors
  Action_UPDATE_CYCLE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action_UPDATE_CYCLE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // initializer
  void initializeAdditionalData(uint8_t * aDataPointer);

  void runAction() const;  

  void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;
};

#endif
