#ifndef SCHEDULER_ACTIONS_SEND_MEASURE_H_
#define SCHEDULER_ACTIONS_SEND_MEASURE_H_

#include "../../Action.h"

class Action_SEND_MEASURE : public Action {
  public:
  // Constructors
  Action_SEND_MEASURE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
  Action_SEND_MEASURE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer);

  // initializer
  void runAction() const;  

  void addAdditionalActionData(char * argument, uint8_t argumentNumber) const;
};

#endif
