#include "../../../Debug_Lib.h"

#include "Action_EMPTY_ACTION.h"

Action_EMPTY_ACTION::Action_EMPTY_ACTION(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {

}

Action_EMPTY_ACTION::Action_EMPTY_ACTION(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {

}

void Action_EMPTY_ACTION::runAction() const {
}

void Action_EMPTY_ACTION::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  switch (argumentNumber) {
    default:
      debug("Unknown argument added to the EMPTY_ACTION action: "+String(argument));
      break;
  }  
}
