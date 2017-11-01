#ifndef WIFI_RTCDATA_H_
#define WIFI_RTCDATA_H_

#include "Arduino.h"

class Wifi_RTCData {
  private:
  // 4 bytes: Timestamp of the last sent measure
  time_t timestamp; 

  public:
  // Getters
  time_t getTimestamp() const;

  // Setters
  void setTimestamp(time_t newTimestamp);

  // Elaborate setters
  void increaseTimestamp(uint16_t timeIncrease);
  void increaseTimestamp(int32_t timeDiff);
};
#endif
