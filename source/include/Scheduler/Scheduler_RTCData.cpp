#include "Scheduler_RTCData.h"

#include "Action.h"

Scheduler_RTCData::Scheduler_RTCData() {
  this->tMask = *(new TimeMask());
  this->aMask = 0;
}

uint8_t Scheduler_RTCData::getLastDayOfExecution() const {
  return (this->aMask & 0x70 >> 3);
}

Action * Scheduler_RTCData::getAction() {
  return new Action(&(this->tMask),(bool)((this->aMask & 0x80) >> 7));
}

void Scheduler_RTCData::updateLastDayOfExecution(uint8_t day) {
  this->aMask = ((this->aMask & 0x8F) | ((day & 0x7) << 4));
}

void Scheduler_RTCData::updateActionData(Action * action) {
  this->tMask = *action->getTimeMask();
}
