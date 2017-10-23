#include "Action.h"
#include "ActionStaticFunctions.h"

#include "Scheduler_RTCData.h"

Scheduler_RTCData::Scheduler_RTCData() {
  this->tMask = *(new TimeMask());
  this->aMask = *(new ActionMask());
}

uint8_t Scheduler_RTCData::getLastDayOfExecution() const {
  return (this->aMask.getSecondaryData());
}

Action * Scheduler_RTCData::getAction() {
  // If the timemask is empty (should not be)
  if (this->aMask.isEmpty()) {
    return NULL;
  } else {
    return getNewAction(&(this->tMask),&(this->aMask), NULL);
  }
}

void Scheduler_RTCData::updateLastDayOfExecution(uint8_t day) {
  this->aMask.setSecondaryData(day);
}

void Scheduler_RTCData::updateActionData(Action * action) {
  // Save the secondary data first
  uint8_t secondaryData = this->aMask.getSecondaryData();

  // Verify if the action is NULL
  if (action == NULL) {
    // if so, save as empty ActionMask
    *(this->aMask.getRawData()) = 0;
  } else {
    // else save the timemask and actionmask
    this->tMask = *action->getTimeMask();
    this->aMask = *action->getActionMask();
  }

  // Save again the secondary data
  this->aMask.setSecondaryData( secondaryData);
}
