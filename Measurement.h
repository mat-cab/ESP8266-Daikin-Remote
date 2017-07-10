#ifndef Measurement_H_
#define Measurement_H_

#include "Arduino.h"

// structures for the meaasurement (stored in EEPROM)
struct measurement {
  uint16_t iterationMoment;
  byte humidity[2];
  byte temperature[2];
  uint16_t voltage;
};

#endif
