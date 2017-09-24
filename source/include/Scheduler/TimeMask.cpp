#include "Debug_Lib.h"

#include "TimeMask.h"

TimeMask::TimeMask() {
  this->tMask[0] = 0;
  this->tMask[1] = 0;
  this->tMask[2] = 0;
}

TimeMask::TimeMask(uint8_t * dataPointer) {
  this->tMask[0] = *dataPointer;
  this->tMask[1] = *(dataPointer + 1);
  this->tMask[2] = *(dataPointer + 2);
}

uint8_t * TimeMask::getRawData() const {
  return ((uint8_t*)(this->tMask));
}

DaysMask TimeMask::getDaysMask() const {
  return (this->tMask[0] >> 1);
}

uint8_t TimeMask::getHour()  const {
  return (((this->tMask[0] & 0x01) << 4) | ((this->tMask[1] & 0xF0) >> 4));
}

uint8_t TimeMask::getMinute() const {
  return (((this->tMask[1] & 0x0F) << 2) | ((this->tMask[2] & 0xC0) >> 6));
}

uint8_t TimeMask::getSecond() const {
  return (this->tMask[2] & 0x3F);
}

void TimeMask::setDaysMask(DaysMask *dMask) {
  this->tMask[0] = ((this->tMask[0] & 0x01) | (*dMask << 1));
}

void TimeMask::setHour(uint8_t hour) {
  this->tMask[0] = ((this->tMask[0] & 0xFE) | ((hour >> 4) & 0x01));
  this->tMask[1] = ((this->tMask[1] & 0x0F) | (hour << 4));
}

void TimeMask::setMinute(uint8_t minute) {
  this->tMask[1] = ((this->tMask[1] & 0xF0) | ((minute >> 2) & 0x0F));
  this->tMask[2] = ((this->tMask[2] & 0x3F) | (minute << 6));
}

void TimeMask::setSecond(uint8_t second) {
  this->tMask[2] = ((this->tMask[2] & 0xC0) | (second & 0x3F));
}

TimeMask * getTimeMask( DaysMask dMask, uint8_t hour, uint8_t minute, uint8_t second) {
  TimeMask result;
  
  result.setDaysMask(&dMask);
  result.setHour(hour);
  result.setMinute(minute);
  result.setSecond(second);

  return &result;
}
