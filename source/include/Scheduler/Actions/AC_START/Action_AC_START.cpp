#include "../../../Debug_Lib.h"
#include "../../../IR/IR_Lib.h"

#include "Action_AC_START.h"

#include "AdditionalData_AC_START.h"

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

  // This part is the protocol specific part!
  switch (getIRProtocol()) {
    case DAIKIN_PROTOCOL:
      IRsetBooleanOption("SwingVertical", actionData->getBooleanOption(0));
      IRsetBooleanOption("SwingHorizontal", actionData->getBooleanOption(1));
      IRsetBooleanOption("Powerful", actionData->getBooleanOption(2));
      IRsetBooleanOption("Silent", actionData->getBooleanOption(3));
      IRsetBooleanOption("Powerful", actionData->getBooleanOption(4));
      IRsetBooleanOption("Sensor", actionData->getBooleanOption(5));
      break;
    case COOLIX_PROTOCOL:
      IRsetBooleanOption("Led", actionData->getBooleanOption(0));
      IRsetBooleanOption("Swing", actionData->getBooleanOption(1));
      IRsetBooleanOption("Sleep", actionData->getBooleanOption(2));
      IRsetBooleanOption("Turbo", actionData->getBooleanOption(3));
      IRsetBooleanOption("Clean", actionData->getBooleanOption(4));
      break;
  }

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
      actionData->setFanSpeed(argument);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      actionData->setBooleanOption(argumentNumber - 4, strcmp( argument, "1") == 0);
      break;
    default:
      debug("Unknown argument added to the AC_START action: "+String(argument));
      break;
  }  
}
