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

void writeMeasurementInEEPROM(struct measurement *measureDatastore);

void readMeasurementFromEEPROM( uint16_t measurementIndex, struct measurement *measureDatastore);
#endif
