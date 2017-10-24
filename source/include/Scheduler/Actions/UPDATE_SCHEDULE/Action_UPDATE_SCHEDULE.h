#ifndef SCHEDULER_ACTIONS_UPDATE_SCHEDULE_H_
#define SCHEDULER_ACTIONS_UPDATE_SCHEDULE_H_

#include "../../Action.h"

class Action_UPDATE_SCHEDULE : public Action {
  public:
  // Constructors
  Action_UPDATE_SCHEDULE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action_UPDATE_SCHEDULE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // initializer
  void runAction() const;  

  void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;
};

#endif
