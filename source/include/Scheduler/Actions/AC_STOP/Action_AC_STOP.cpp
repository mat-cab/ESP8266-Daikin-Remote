#include "../../../Debug_Lib.h"
#include "../../../IR_Lib.h"

#include "Action_AC_STOP.h"

Action_AC_STOP::Action_AC_STOP(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {

}

Action_AC_STOP::Action_AC_STOP(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {

}

void Action_AC_STOP::runAction() const {
  sendIRStop();
}

void Action_AC_STOP::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  switch (argumentNumber) {
    default:
      debug("Unknown argument added to the AC_STOP action: "+String(argument));
      break;
  }  
}
