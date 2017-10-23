#include "../../../Debug_Lib.h"
#include "../../../CycleManager/CycleManager.h"

#include "AdditionalData_UPDATE_CYCLE.h"

#include "Action_UPDATE_CYCLE.h"

Action_UPDATE_CYCLE::Action_UPDATE_CYCLE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {
  this->initializeAdditionalData(NULL);
}

Action_UPDATE_CYCLE::Action_UPDATE_CYCLE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {
  this->initializeAdditionalData(aDataPointer);
}

void Action_UPDATE_CYCLE::initializeAdditionalData(uint8_t * aDataPointer) {
  if ( aDataPointer = NULL ) {
    this->aData = new AdditionalData_UPDATE_CYCLE();
  } else {
    this->aData = new AdditionalData_UPDATE_CYCLE(aDataPointer);
  }
}

void Action_UPDATE_CYCLE::runAction() const {
  debug("Updating the cycle time to "+String(((AdditionalData_UPDATE_CYCLE*)this->aData)->getCycleTime())+" ms");
}

void Action_UPDATE_CYCLE::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  switch (argumentNumber) {
    case 1:
      ((AdditionalData_UPDATE_CYCLE*)aData)->setCycleTime(atoi(argument));
      break;
    default:
      debug("Unknown argument added to the UPDATE_CYCLE action: "+String(argument));
      break;
  }  
}
