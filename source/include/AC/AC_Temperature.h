#ifndef AC_TEMPERATURE_H_
#define AC_TEMPERATURE_H_

#include "Arduino.h"

uint8_t getProtocolTemperature(uint8_t temperature);

uint8_t getAbsoluteTemperature(uint8_t protocolTemperature);
#endif 
