#include "../../../Debug_Lib.h"
#include "../../../IR/IR_Lib.h"

#include "AdditionalData_AC_START.h"

#include "Action_AC_START.h"

Action_AC_START::Action_AC_START(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {
  this->initializeAdditionalData(NULL);
}

Action_AC_START::Action_AC_START(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {
  this->initializeAdditionalData(aDataPointer);
}

void Action_AC_START::initializeAdditionalData(uint8_t * aDataPointer) {
  if ( aDataPointer == NULL ) {
    this->aData = new AdditionalData_AC_START();
  } else {
    this->aData = new AdditionalData_AC_START(aDataPointer);
  }
}

void Action_AC_START::runAction() const {
  AdditionalData_AC_START *actionData = (AdditionalData_AC_START *)this->aData;

  IRsetOn();
  IRsetMode(actionData->getACMode());
  IRsetTemperature(actionData->getTemperature());
  IRsetFanSpeed(actionData->getFanSpeed());
  IRsetBooleanOption("SwingVertical", actionData->getSwingLR());
  IRsetBooleanOption("SwingHorizontal", actionData->getSwingUD());
  IRsetBooleanOption("Powerful", actionData->getPowerful());
  IRsetBooleanOption("Silent", actionData->getSilent());

  IRsendCommand(); 
}

void Action_AC_START::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  AdditionalData_AC_START *actionData = (AdditionalData_AC_START *)this->aData;

  switch (argumentNumber) {
    case 1:
      actionData->setACMode(argument);
      break;
    case 2:
      actionData->setTemperature(atoi(argument));
      break;
    case 3:
      actionData->setFanSpeed(atoi(argument));
      break;
    case 4:
      actionData->setSwingLR(strcmp( argument, "1") == 0);
      break;
    case 5:
      actionData->setSwingUD(strcmp( argument, "1") == 0);
      break;
    case 6:
      actionData->setPowerful(strcmp( argument, "1") == 0);
      break;
    case 7:
      actionData->setSilent(strcmp( argument, "1") == 0);
      break;
    case 8:
      actionData->setIntelligentEye(strcmp( argument, "1") == 0);
      break;
    default:
      debug("Unknown argument added to the AC_START action: "+String(argument));
      break;
  }  
}
