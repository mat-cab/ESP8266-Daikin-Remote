#include "Wifi_RTCData.h"

time_t Wifi_RTCData::getTimestamp() const {
  return this->timestamp;
}

void Wifi_RTCData::setTimestamp(time_t newTimestamp) {
  this->timestamp = newTimestamp;
}

void Wifi_RTCData::increaseTimestamp(uint16_t timeIncrease) {
  this->timestamp += timeIncrease;
}

void Wifi_RTCData::increaseTimestamp(int32_t timeDiff) {
  this->timestamp += timeDiff;
}
