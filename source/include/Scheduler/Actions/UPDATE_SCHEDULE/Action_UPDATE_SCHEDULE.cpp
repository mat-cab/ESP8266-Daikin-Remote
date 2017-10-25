#include "../../../Debug_Lib.h"

#include "../../../Wifi/Wifi.h"

#include "Action_UPDATE_SCHEDULE.h"

Action_UPDATE_SCHEDULE::Action_UPDATE_SCHEDULE(ActionType actionType, DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) : Action(actionType, dMask, hour, minute, second) {

}

Action_UPDATE_SCHEDULE::Action_UPDATE_SCHEDULE(TimeMask *tMask, ActionMask *aMask, uint8_t *aDataPointer) : Action(tMask, aMask, aDataPointer) {

}

void Action_UPDATE_SCHEDULE::runAction() const {
  // First connect to the Wifi
  connectToWifi();

  // Then get the schedule from the server
  receiveWifi();
}

void Action_UPDATE_SCHEDULE::addAdditionalActionData(char * argument, uint8_t argumentNumber) const {
  switch (argumentNumber) {
    default:
      debug("Unknown argument added to the UPDATE_SCHEDULE action: "+String(argument));
      break;
  }  
}
