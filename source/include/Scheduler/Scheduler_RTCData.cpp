#include "Action.h"

#include "Scheduler_RTCData.h"

Scheduler_RTCData::Scheduler_RTCData() {
  this->tMask = *(new TimeMask());
  this->aMask = *(new ActionMask());
}

uint8_t Scheduler_RTCData::getLastDayOfExecution() const {
  return (this->aMask.getSecondaryData());
}

Action * Scheduler_RTCData::getAction() {
  return new Action(&(this->tMask),&(this->aMask));
}

void Scheduler_RTCData::updateLastDayOfExecution(uint8_t day) {
  this->aMask.setSecondaryData(day);
}

void Scheduler_RTCData::updateActionData(Action * action) {
  // Save the secondary data first
  uint8_t secondaryData = this->aMask.getSecondaryData();

  this->tMask = *action->getTimeMask();
  this->aMask = *action->getActionMask();

  // Save again the secondary data
  this->aMask.setSecondaryData( secondaryData);
}
