#ifndef SCHEDULER_ACTIONS_AC_START_H_
#define SCHEDULER_ACTIONS_AC_START_H_

#include "../../Action.h"

class Action_AC_START : public Action {
  public:
  // Constructors
  Action_AC_START(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action_AC_START(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // initializer
  void initializeAdditionalData(uint8_t * aDataPointer);

  void runAction() const;  

  void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;
};

#endif
