#include "../../../Debug_Lib.h"

#include "Action_SEND_MEASURE.h"

Action_SEND_MEASURE::Action_SEND_MEASURE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {

}

Action_SEND_MEASURE::Action_SEND_MEASURE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {

}

void Action_SEND_MEASURE::runAction() const {
  debug("Sending the SEND_MEASURE command!");
}

void Action_SEND_MEASURE::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  switch (argumentNumber) {
    default:
      debug("Unknown argument added to the SEND_MEASURE action: "+String(argument));
      break;
  }  
}
