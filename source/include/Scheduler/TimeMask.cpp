#include "Debug_Lib.h"

#include "TimeMask.h"

TimeMask::TimeMask() {
  tMask[0] = 0;
  tMask[1] = 0;
  tMask[2] = 0;
  tMask[3] = 0;
}

void TimeMask::setDaysMask(DaysMask *dMask) {
  tMask[0] = ((tMask[0] & 0x01) | (*dMask << 1));
}

void TimeMask::setHour(uint8_t hour) {
  tMask[0] = ((tMask[0] & 0xFE) | ((hour >> 4) & 0x01));
  tMask[1] = ((tMask[1] & 0x0F) | (hour << 4));
}

void TimeMask::setMinute(uint8_t minute) {
  tMask[1] = ((tMask[1] & 0xF0) | ((minute >> 2) & 0x0F));
  tMask[2] = ((tMask[2] & 0x3F) | (minute << 6));
}

void TimeMask::setSecond(uint8_t second) {
  tMask[2] = ((tMask[2] & 0xC0) | (second & 0x3F));
}

DaysMask TimeMask::getDaysMask() {
  return (tMask[0] >> 1);
}

uint8_t TimeMask::getHour() {
  return (((tMask[0] & 0x01) << 4) | ((tMask[1] & 0xF0) >> 4));
}

uint8_t TimeMask::getMinute() {
  return (((tMask[1] & 0x0F) << 2) | ((tMask[2] & 0xC0) >> 6));
}

uint8_t TimeMask::getSecond() {
  return (tMask[2] & 0x3F);
}

TimeMask * getTimeMask( DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) {
  TimeMask result;
  
  result.setDaysMask(&dMask);
  result.setHour(hour);
  result.setMinute(minute);
  result.setSecond(second);

  return &result;
}
