#ifndef SCHEDULER_ACTION_STATIC_FUNCTIONS_H_
#define SCHEDULER_ACTION_STATIC_FUNCTIONS_H_

#include "Arduino.h"

#include "Action.h"
#include "TimeMask.h"
#include "ActionMask.h"

// Static methods
Action * getNewAction(TimeMask * timeMask, ActionMask * aMask, uint8_t * aDataPointer);
Action * getNewAction(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
Action * parseActionFromString(String actionString);

#endif
