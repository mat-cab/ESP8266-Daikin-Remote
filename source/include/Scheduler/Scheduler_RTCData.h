#ifndef SCHEDULER_RTCDATA_H_
#define SCHEDULER_RTCDATA_H_

#include "Arduino.h"

#include "Action.h"

class Scheduler_RTCData {
  private:
  // RTC data storage structure for the scheduler
  // 4 bytes: TimeMask of the next action
  TimeMask tMask;
  // 1 bytes for the action mask
  ActionMask aMask;

  public:
  // Constructors
  Scheduler_RTCData();

  // Getters
  uint8_t getLastDayOfExecution() const;

  // Elaborate getters
  Action * getAction();

  // Setters
  void updateLastDayOfExecution(uint8_t day);
  void updateActionData(Action * action);
};
#endif
