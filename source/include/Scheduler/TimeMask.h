#ifndef SCHEDULER_TIMEMASK_H_
#define SCHEDULER_TIMEMASK_H_

#include "Arduino.h"

// Type of the DateMask (only for days)
// Bit 0 for Sunday
// Bit 1 for Monday
// ...
// Bit 6 for Saturday
// Bit 7 unused
typedef uint8_t DaysMask;

class TimeMask {
  // Attributes
  private:
  // Data storage of the time mask
  // Bits 0 to 6 : execute on day x
  // Bits 7 to 11: execute at hour h
  // Bits 12 to 17 : execute at minute m
  // Bits 18 to 23 : execute at second s
  uint8_t tMask[4];

  // Public methods
  public:
  // Constructors
  TimeMask();
//  TimeMask(DaysMask *dMask, uint8_t hour, uint8_t minute, uint8_t second);
  
  void setDaysMask(DaysMask *dMask);

  void setHour(uint8_t hour);

  void setMinute(uint8_t minute);

  void setSecond(uint8_t second);

  DaysMask getDaysMask();

  uint8_t getHour();

  uint8_t getMinute();

  uint8_t getSecond();
};

TimeMask * getTimeMask( DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second);
#endif
